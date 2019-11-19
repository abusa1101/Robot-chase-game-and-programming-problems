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
}

void move(robot_t *robot) {
    double fwd_vel = min(MAX_VEL, robot->fwd_vel);
    double xdist = fwd_vel * cos(robot->theta);
    double ydist = fwd_vel * -sin(robot->theta);
    robot->x += xdist;
    robot->y += ydist;
}

void chaser_movement(state_t *state) {
    int action = state->user_action;
    if (action == 1) { //up
        state->chaser.fwd_vel += 4;
        if (state->chaser.fwd_vel > MAX_VEL) {
            state->chaser.fwd_vel = MAX_VEL;
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

// void potential_field_control(void) {
//     // circle approximation radius of the robot
//     robot_r = sqrt((robot_width / 2)^2 + (robot_height / 2)^2)
//     // circle approximation radius of a wall block
//     wall_r = block_width / sqrt(2)
//
//     fx = 0.0 // forces on the robot
//     fy = 0.0
//
//     to_goal = unit vector from the chaser to the runner
//     to_goal_dist = distance from the chaser to the runner
//     fx, fy += to_goal * to_goal_magnitude * pow(to_goal_dist, to_goal_power)
//
//     for each wall block {
//         to_runner = unit vector from the wall block to the chaser
//         to_obs_dist = distance between the runner and the chaser, approximating them as circles
//         to_obs_dist = fmax(0.1, to_obs_dist)
//         fx, fy += to_runner * avoid_obs_magnitude * pow(to_obs_dist, avoid_obs_power)
//     }
//
//     target_theta = theta of fx, fy vector
//     theta_error = target_theta - robot theta, constrained to range [-pi, pi]
//     ang_velocity = 0.4 * theta_error, constrained to range [-pi / 16, pi / 16]
//
//     velocity = fmin(max_velocity, velocity + 2.0)
// }
