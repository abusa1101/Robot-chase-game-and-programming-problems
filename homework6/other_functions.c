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
    return a < b ? a : b;
}

int constrain(int val, int LL, int UL) {
    val = (val < LL) ? LL : val;
    val = (val > UL) ? UL : val;
    return val;
}

void bmp_init(bitmap_t *bmp) {
    bmp->width = WIDTH;
    bmp->height = HEIGHT;
    bmp->data = calloc(bmp->width * bmp->height, sizeof(color_bgr_t));
}

int change_robot_idx(int idx, bool is_next) {
    if (is_next) {
        do {
            idx++;
            idx = constrain(idx, 17, 175);
        } while (MAP[idx] == 'X');
    } else {
        do {
            idx--;
            idx = constrain(idx, 17, 175);
        } while (MAP[idx] == 'X');
    }
    return idx;
}

void serving_img(bitmap_t bmp, state_t *state) {
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(&bmp, serialized_bmp);
    image_server_set_data(bmp_size, serialized_bmp);
    free(serialized_bmp);
    int seconds = 0;
    long nanoseconds = SLEEP_40 * 1000 * 1000; //FIX THIS- HOW TO USE DELAY_START?
    struct timespec interval = {seconds, nanoseconds};
    nanosleep(&interval, NULL);
}

//Movement
void init_values(state_t *state) {
    state->chaser.theta = 0;
    state->chaser.x = (double)WIDTH / 2;
    state->chaser.y = (double)HEIGHT / 2;
    //state->runner.theta = 0;
    state->initial_runner_idx = 17;
    give_runner_pos(state, state->initial_runner_idx);
    state->delay_every = 1;
    state->to_goal_magnitude = 50.0;
    state->to_goal_power = 0;
    state->avoid_obs_magnitude = 1.0;
    state->avoid_obs_power = -2;
    state->max_velocity = 12;
    state->current_parameter = 1;
}

void runner_walks(state_t *state) {
    int num_chosen = rand() % 20;
    if (num_chosen == 1) {
        state->runner.fwd_vel += 2;
    } else if (num_chosen == 2) {
        state->runner.ang_vel += (M_PI / 16);
    }
    state->runner.theta += state->runner.ang_vel;
    state->runner.ang_vel *= 0.8;
    move(state, &state->runner);
}

void move(state_t *state, robot_t *robot) {
    double fwd_vel = fmin(state->max_velocity, robot->fwd_vel);
    //printf("fwd_vel: %lf\n", fwd_vel);
    double xdist = fwd_vel * cos(robot->theta);
    double ydist = fwd_vel * -sin(robot->theta);
    robot->x += xdist;
    robot->y += ydist;
}

void chaser_moves(state_t *state) {
    potential_field_control(state);
    state->chaser.theta += state->chaser.ang_vel;
    state->chaser.ang_vel *= 0.8;
    move(state, &state->chaser);
}

//Potential Field
void potential_field_control(state_t *state) {
    double robot_r = sqrt(pow((ROB_W / 2.0), 2) + pow((ROB_L / 2.0), 2));
    double wall_r = BLOCK_SIZE / sqrt(2);
    double fx = 0.0; //forces on robot
    double fy = 0.0;

    double dist_sq_robots = pow(state->runner.x - state->chaser.x, 2) +
                            pow(state->runner.y - state->chaser.y, 2);
    double to_goal_x = (state->runner.x - state->chaser.x) / sqrt(dist_sq_robots);
    double to_goal_y = (state->runner.y - state->chaser.y) / sqrt(dist_sq_robots);
    double to_goal_dist = sqrt(pow(state->runner.x - state->chaser.x, 2) +
                               pow(state->runner.y - state->chaser.y, 2));

    fx += to_goal_x * state->to_goal_magnitude *
          pow(to_goal_dist, state->to_goal_power);
    fy += to_goal_y * state->to_goal_magnitude *
          pow(to_goal_dist, state->to_goal_power);

    for (int i = 0; i < MAP_H * MAP_W; i++) {
        if (MAP[i] == 'X') {
            int x = ((i % MAP_W) * BLOCK_SIZE) + (BLOCK_SIZE / 2);
            int y = ((i / MAP_W) * BLOCK_SIZE) + (BLOCK_SIZE / 2);

            double dist_sq = pow(state->chaser.x - x, 2) + pow(state->chaser.y - y, 2);
            double to_chaser_x = (state->chaser.x - x) / sqrt(dist_sq);
            double to_chaser_y = (state->chaser.y - y) / sqrt(dist_sq);
            double to_obs_dist = sqrt(dist_sq) - (robot_r + wall_r);
            to_obs_dist = fmax(0.1, to_obs_dist);
            fx += to_chaser_x * state->avoid_obs_magnitude *
                  pow(to_obs_dist, state->avoid_obs_power);
            fy += to_chaser_y * state->avoid_obs_magnitude *
                  pow(to_obs_dist, state->avoid_obs_power);
            //printf("fx, fy: %lf, %lf\n", fx, fy);
        }
    }

    double target_theta = atan2(-fy, fx);
    double theta_error = target_theta - state->chaser.theta;
    while (theta_error > M_PI) {
        theta_error -= 2 * M_PI;
    }
    while (theta_error < -M_PI) {
        theta_error += 2 * M_PI;
    }
    double ang_vel = 0.4 * theta_error;
    // printf("ang vel: %lf\n", ang_vel);
    ang_vel = (ang_vel < -M_PI / 16) ? -M_PI / 16 : ang_vel;
    ang_vel = (ang_vel > M_PI / 16) ? M_PI / 16 : ang_vel;
    state->chaser.ang_vel = ang_vel;

    state->chaser.fwd_vel = fmin(state->max_velocity, state->chaser.fwd_vel + 2.0);
}

void update_parameters(state_t *state, bool action_is_up) {
    if (action_is_up) {
        if (state->current_parameter == 1) {
            state->initial_runner_idx = change_robot_idx(state->initial_runner_idx, 1);
        } else if (state->current_parameter == 2) {
            state->delay_every += 1;
            state->delay_every = constrain(state->delay_every, 1, 10000000);
        } else if (state->current_parameter == 3) {
            state->to_goal_magnitude *= 2;
        } else if (state->current_parameter == 4) {
            state->to_goal_power += 1;
            state->to_goal_power = constrain(state->to_goal_power, -3, 3);
        } else if (state->current_parameter == 5) {
            state->avoid_obs_magnitude *= 2;
        } else if (state->current_parameter == 6) {
            state->avoid_obs_power += 1;
            state->avoid_obs_power = constrain(state->avoid_obs_power, -3, 3);
        } else if (state->current_parameter == 7) {
            state->max_velocity += 1;
            state->max_velocity = constrain(state->max_velocity, 1, 12);
        } else {
            printf("UP Error: %d Check IO_thread\n", state->current_parameter);
        }
    } else { //action = down
        if (state->current_parameter == 1) {
            state->initial_runner_idx = change_robot_idx(state->initial_runner_idx, 0);
        } else if (state->current_parameter == 2) {
            state->delay_every -= 1;
            state->delay_every = constrain(state->delay_every, 1, 10000000); //inf basically
        } else if (state->current_parameter == 3) {
            state->to_goal_magnitude /= 2;
        } else if (state->current_parameter == 4) {
            state->to_goal_power -= 1;
            state->to_goal_power = constrain(state->to_goal_power, -3, 3);
        } else if (state->current_parameter == 5) {
            state->avoid_obs_magnitude /= 2;
        } else if (state->current_parameter == 6) {
            state->avoid_obs_power -= 1;
            state->avoid_obs_power = constrain(state->avoid_obs_power, -3, 3);
        } else if (state->current_parameter == 7) {
            state->max_velocity -= 1;
            state->max_velocity = constrain(state->max_velocity, 1, 12);
        } else {
            printf("DOWN Error: %d Check IO_thread\n", state->current_parameter);
        }
    }
}

void reset_simulation(state_t *state) {
    srand(0);
    state->chaser.theta = 0;
    state->chaser.x = WIDTH / 2.0;
    state->chaser.y = HEIGHT / 2.0;
    state->timestep = 0;
    give_runner_pos(state, state->initial_runner_idx);
    // state->runner.x = (double)((state->initial_runner_idx % MAP_W) + 0.5) * BLOCK_SIZE;
    // state->runner.y = (double)((state->initial_runner_idx / MAP_W) + 0.5) * BLOCK_SIZE;
    state->runner.fwd_vel = 0;
    state->runner.ang_vel = 0;
    state->runner.theta = 0;
    state->chaser.fwd_vel = 0;
    state->chaser.ang_vel = 0;
    state->chaser.theta = 0;
}

//Threading/IO
void reset_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
    printf("\e[?25h");
}

void *io_thread(void *user) {
    printf("\e[?25l");

    tcgetattr(0, &original_termios);
    atexit(reset_terminal);
    struct termios new_termios;
    memcpy(&new_termios, &original_termios, sizeof(new_termios));
    new_termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    state_t *state = user;
    while (true) {
        int c = getc(stdin);
        if (c == 'q') {
            exit(0);
        }
        if (c == 'r') {
            reset_simulation(state); //when r is pressed
        }
        if (c == '\e' && getc(stdin) == '[') {
            c = getc(stdin);
            if (c == 'A') { //up
                update_parameters(state, 1);
            } else if (c == 'B') { //down
                update_parameters(state, 0);
            } else if (c == 'D') { //left
                state->current_parameter--;
                state->current_parameter = constrain(state->current_parameter, 1, 7);
            } else if (c == 'C') { // right
                state->current_parameter++;
                state->current_parameter = constrain(state->current_parameter, 1, 7);
            } else {
                continue;
            }
        }
    }
}
