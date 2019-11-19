#define _GNU_SOURCE
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>
#include <termios.h>
#include "bmp.h"
#include "all_functions.h"

struct termios original_termios;

//Vector/low-level Operations
vector_xy_t *vector_create(void) {
    vector_xy_t *v = malloc(sizeof(vector_xy_t));
    v->capacity = 8;
    v->size = 0;
    v->data = malloc(sizeof(points_t) * v->capacity);
    return v;
}

void vector_append(vector_xy_t *v, double xvalue, double yvalue) {
    if (v->capacity == v->size) {
        v->data = realloc(v->data, sizeof(points_t) * (2 * v->capacity));
        v->capacity *= 2;
    }
    v->data[v->size].x = xvalue;
    v->data[v->size].y = yvalue;
    v->size++;
}

void vector_free(vector_xy_t *v) {
    free(v->data);
    free(v);
}

double max(double a, double b) {
    double c;
    if (a >= b) {
        c = a;
    } else {
        c = b;
    }
    return c;
}

double min(double a, double b) {
    double c;
    if (a <= b) {
        c = a;
    } else {
        c = b;
    }
    return c;
}

//Threading/IO
void reset_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

void *io_thread(void *user) {
    state_t *state = user;
    tcgetattr(0, &original_termios);
    atexit(reset_terminal);
    struct termios new_termios;
    memcpy(&new_termios, &original_termios, sizeof(new_termios));
    new_termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    while (true) {
        int c = getc(stdin);
        if (c == 'q') {
            exit(0);
        }
        if (c == 'r') {
            //reset simulation
        }
        if (c == '\e' && getc(stdin) == '[') {
            c = getc(stdin);
            if (c == 'A') {
                state->user_action = 1; //up
            } else if (c == 'B') {
                state->user_action = 4; //down
            } else if (c == 'C') {
                state->user_action = 2; //left
            } else if (c == 'D') {
                state->user_action = 3; // right
            } else {
                continue;
            }
        }
    }
}


//Movement
void init_values(bitmap_t *bmp, state_t *state) {
    bmp->width = 640;
    bmp->height = 480;
    bmp->data = calloc(bmp->width * bmp->height, sizeof(color_bgr_t));
    state->chaser.theta = 0;
    state->chaser.x = 320;
    state->chaser.y = 240;
    state->runner.theta = 0;
    state->runner.x = 0;
    state->runner.y = 0;
    state->runner.is_runner = true;
    state->initial_runner_idx = 17;
    state->delay_every = 1;
    state->to_goal_magnitude = 50.0;
    state->to_goal_power = 0;
    state->avoid_obs_magnitude 50.0;
    state->avoid_obs_power = -2;
    state->max_velocity = 12;
}

void serving_img(bitmap_t bmp) {
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(&bmp, serialized_bmp);
    image_server_set_data(bmp_size, serialized_bmp);
    free(serialized_bmp);
    int seconds = 0;
    long nanoseconds = 40 * 1000 * 1000;
    struct timespec interval = {seconds, nanoseconds};
    nanosleep(&interval, NULL);
}

void move(robot_t *robot) {
    double fwd_vel = min(state->max_velocity, robot->fwd_vel);
    double xdist = fwd_vel * cos(robot->theta);
    double ydist = fwd_vel * -sin(robot->theta);
    robot->x += xdist;
    robot->y += ydist;
}

void runner_walks(robot_t *runner) {
    int num_chosen = rand() % 20; //[max = 19, min = 0]
    if (num_chosen == 1) {
        runner->fwd_vel += 2;
        //printf("%d ", num_chosen);
    } else if (num_chosen == 2) {
        runner->ang_vel += (M_PI / 16);
        //printf("%d ", num_chosen);
    } else {
        //printf("%d ", 0);
    }
    runner->theta += runner->ang_vel;
    runner->ang_vel *= 0.8;
    move(runner);
    resolve_tile_collision(runner);
}

void chaser_movement(state_t *state) {
    int action = state->user_action;
    if (action == 1) { //up
        state->chaser.fwd_vel += 4;
        if (state->chaser.fwd_vel > state->max_velocity) {
            state->chaser.fwd_vel = state->max_velocity;
        }
    } else if (action == 2) { //left
        state->chaser.ang_vel -= M_PI / 32;
    } else if (action == 3) { //right
        state->chaser.ang_vel += M_PI / 32;
    }
    state->chaser.theta += state->chaser.ang_vel;
    state->chaser.ang_vel *= 0.8;
    move(&state->chaser);
    if (resolve_tile_collision(&state->chaser)) {
        state->chaser.fwd_vel *= 0.25;
    }
    state->user_action = 0;
}

void play_game(state_t *state) {
    runner_walks(&state->runner);
    chaser_movement(state);
}

void potential_field_control(state_t *state) {
    double robot_r = sqrt((ROB_W / 2)^2 + (ROB_L / 2)^2); //circle approx radius of robot
    double wall_r = BLOCK_SIZE / sqrt(2); //circle approx radius of wall block

    double fx = 0.0; //forces on robot
    double fy = 0.0;

    double dist_sq = pow(state->runner.x - state->chaser.x, 2) +
                             pow(state->runner.y - state->chaser.y, 2);
    double to_goal_x = (state->runner.x - state->chaser.x) / sqrt(dist_sq); //unit vec from chaser to runner
    double to_goal_dist_x = state->runner.x - state->chaser.x; //dist from chaser to runner
    double to_goal_y = (state->runner.y - state->chaser.y) / sqrt(dist_sq);
    double to_goal_dist_y = state->runner.y - state->chaser.y;

    fx += to_goal_x * state->to_goal_magnitude * pow(to_goal_dist_x, state->to_goal_power);
    fy += to_goal_y * state->to_goal_magnitude * pow(to_goal_dist_y, state->to_goal_power);

    for each wall block {
        to_runner = //unit vector from the wall block to the chaser
        to_obs_dist = //distance between the runner and the chaser, approximating them as circles
        to_obs_dist = fmax(0.1, to_obs_dist)
        fx += to_runner * state->avoid_obs_magnitude * pow(to_obs_dist, state->avoid_obs_power);
        fy += to_runner * state->avoid_obs_magnitude * pow(to_obs_dist, state->avoid_obs_power);
    }

    double target_theta = theta of fx, fy vector
    theta_error = target_theta - state->chaser.theta; //constrained to range [-pi, pi]
    ang_velocity = 0.4 * theta_error //constrained to range [-pi / 16, pi / 16]

    state->chaser.fwd_vel = fmin(max_velocity, state->chaser.fwd_vel + 2.0)
}
