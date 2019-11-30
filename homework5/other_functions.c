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

int constrain(int val, int LL, int UL) {
    val = (val < LL) ? LL : val;
    val = (val > UL) ? UL : val;
    return val;
}

void bmp_init(bitmap_t *bmp) {
    bmp->width = 640;
    bmp->height = 480;
    bmp->data = calloc(bmp->width * bmp->height, sizeof(color_bgr_t));
}

int give_robot_idx(double x, double y) {
    int idx = (int)y * MAP_W + (int)x;
    return idx;
}

int move_to_robot_idx(int current_idx, bool is_next) {
    if (MAP[current_idx] == 'X' && is_next) {
        while (MAP[current_idx] == 'X') {
            current_idx++;
            current_idx = constrain(current_idx, 1, 60);
        }
    } else if (MAP[current_idx] == 'X' && !is_next) {
        while (MAP[current_idx] == 'X') {
            current_idx--;
            current_idx = constrain(current_idx, 1, 60); //FIX THIS NUMBER 60 LATER
        }
    }
    return current_idx;
}

//Movement
void init_values(state_t *state) {
    state->chaser.theta = 0;
    state->chaser.x = 320;
    state->chaser.y = 240;
    state->runner.theta = 0;
    state->runner.x = 0;
    state->runner.y = 0;
    state->initial_runner_idx = 17;
    state->delay_every = 1;
    state->to_goal_magnitude = 50.0;
    state->to_goal_power = 0;
    state->avoid_obs_magnitude = 1.0;
    state->avoid_obs_power = -2;
    state->max_velocity = 12;
}

void serving_img(bitmap_t bmp, state_t *state) {
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(&bmp, serialized_bmp);
    image_server_set_data(bmp_size, serialized_bmp);
    free(serialized_bmp);
    int seconds = 0;
    long nanoseconds = 40 * 1000 * 1000; //FIX THIS- HOW TO USE DELAY_START?
    struct timespec interval = {seconds, nanoseconds};
    nanosleep(&interval, NULL);
}

void runner_walks(state_t *state, robot_t *runner) {
    int num_chosen = rand() % 20;
    if (num_chosen == 1) {
        runner->fwd_vel += 2;
    } else if (num_chosen == 2) {
        runner->ang_vel += (M_PI / 16);
    }
    runner->theta += runner->ang_vel;
    runner->ang_vel *= 0.8;
    move(state, runner);
    resolve_tile_collision(runner);
}

void move(state_t *state, robot_t *robot) {
    double fwd_vel = min(state->max_velocity, robot->fwd_vel);
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
    if (resolve_tile_collision(&state->chaser)) {
        state->chaser.fwd_vel *= 0.25;
    }
}

//Potential Field
void potential_field_control(state_t *state) {
    double robot_r = sqrt(pow((ROB_W / 2), 2) + pow((ROB_L / 2), 2));
    double wall_r = BLOCK_SIZE / sqrt(2);
    double fx = 0.0; //forces on robot
    double fy = 0.0;

    double dist_sq_robots = pow(state->runner.x - state->chaser.x, 2) +
                            pow(state->runner.y - state->chaser.y, 2);
    double to_goal_x = (state->runner.x - state->chaser.x) / sqrt(dist_sq_robots)
    double to_goal_y = (state->runner.y - state->chaser.y) / sqrt(dist_sq_robots);
    double to_goal_dist_x = state->runner.x - state->chaser.x; //dist from chaser to runner
    double to_goal_dist_y = state->runner.y - state->chaser.y;

    fx += to_goal_x * state->to_goal_magnitude *
          pow(to_goal_dist_x, state->to_goal_power);
    fy += to_goal_y * state->to_goal_magnitude *
          pow(to_goal_dist_y, state->to_goal_power);

    for (int i = 0; i < MAP_H * MAP_W; i++) {
        if (MAP[i] == 'X') {
            int x = ((i % MAP_W) * BLOCK_SIZE) + (BLOCK_SIZE / 2);
            int y = ((i / MAP_W) * BLOCK_SIZE) + (BLOCK_SIZE / 2);

            double dist_sq = pow(state->chaser.x - x, 2) + pow(state->chaser.y - y, 2);
            double to_chaser_x = (state->chaser.x - x) / sqrt(dist_sq);
            double to_chaser_y = (state->chaser.y - y) / sqrt(dist_sq);
            double to_obs_dist = sqrt(dist_sq_robots) - (robot_r + wall_r); //dist between runner and chaser, approx them as circles
            to_obs_dist = fmax(0.1, to_obs_dist);
            fx += to_chaser_x * state->avoid_obs_magnitude * pow(to_obs_dist, state->avoid_obs_power);
            fy += to_chaser_y * state->avoid_obs_magnitude * pow(to_obs_dist, state->avoid_obs_power);
        }
    }

    double target_theta = atan2(-fy, fx);
    double theta_error = target_theta - state->chaser.theta;
    if (theta_error > M_PI) {
        theta_error = -M_PI + (theta_error - M_PI);
    } else if (theta_error < -M_PI) {
        theta_error = M_PI - (-theta_error - M_PI);
    }
    double ang_vel = 0.4 * theta_error;
    state->chaser.ang_vel = constrain(ang_vel, -M_PI / 16, M_PI / 16);

    state->chaser.fwd_vel = fmin(state->max_velocity, state->chaser.fwd_vel + 2.0);
}

void update_parameters(state_t *state, bool action_is_up) {
    if (action_is_up) {
        if (state->current_parameter == 1) {
            int start_idx = give_robot_idx(state->runner.x, state->runner.y);
            state->initial_runner_idx = move_to_robot_idx(start_idx, 1); //is_next = 1, !is_next = 0
        } else if (state->current_parameter == 2) {
            state->delay_every += 1;
            constrain(state->delay_every, 1, 10000000); //inf basically- FIX do we need this?
        } else if (state->current_parameter == 3) {
            state->to_goal_magnitude *= 2;
        } else if (state->current_parameter == 4) {
            state->to_goal_power += 1;
            constrain(state->to_goal_power, -3, 3);
        } else if (state->current_parameter == 5) {
            state->avoid_obs_magnitude *= 2;
        } else if (state->current_parameter == 6) {
            state->avoid_obs_power += 1;
            constrain(state->avoid_obs_power, -3, 3);
        } else if (state->current_parameter == 7) {
            state->max_velocity += 1;
            constrain(state->max_velocity, 1, 12);
        } else {
            printf("UP Error: No such parameter (> 7). Check IO_thread");
        }
    } else { //action = down
        if (state->current_parameter == 1) {
            int start_idx = give_robot_idx(state->runner.x, state->runner.y);
            state->initial_runner_idx = move_to_robot_idx(start_idx, 0);
        } else if (state->current_parameter == 2) {
            state->delay_every -= 1;
            constrain(state->delay_every, 1, 10000000); //inf basically
        } else if (state->current_parameter == 3) {
            state->to_goal_magnitude /= 2;
        } else if (state->current_parameter == 4) {
            state->to_goal_power -= 1;
            constrain(state->to_goal_power, -3, 3);
        } else if (state->current_parameter == 5) {
            state->avoid_obs_magnitude /= 2;
        } else if (state->current_parameter == 6) {
            state->avoid_obs_power -= 1;
            constrain(state->avoid_obs_power, -3, 3);
        } else if (state->current_parameter == 7) {
            state->max_velocity -= 1;
            constrain(state->max_velocity, 1, 12);
        } else {
            printf("DOWN Error: No such parameter (< 7). Check IO_thread");
        }
    }
}

void reset_simulation(state_t *state) {
    srand(0);
    init_values(state);
    state->initial_runner_idx = give_robot_idx(state->runner.x, state->runner.y);

    state->runner.fwd_vel = 0;
    state->runner.ang_vel = 0;
    state->runner.theta = 0;

    state->chaser.fwd_vel = 0;
    state->chaser.ang_vel = 0;
    state->chaser.theta = 0;

    state->current_parameter = 1;
}

//Threading/IO
void reset_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

void *io_thread(void *user) {
    printf("\e[?25l");
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
            reset_simulation(state); //when r is pressed
        }
        if (c == '\e' && getc(stdin) == '[') {
            c = getc(stdin);
            if (c == 'A') { //up
                state->user_action = 1;
                update_parameters(state, 1);
            } else if (c == 'B') { //down
                state->user_action = 4;
                update_parameters(state, 0);
            } else if (c == 'C') { //left
                state->current_parameter--;
                if (state->current_parameter < 1) {
                    state->current_parameter = 7; //or % maybe?
                }
            } else if (c == 'D') { // right
                state->current_parameter++;
                if (state->current_parameter > 7) {
                    state->current_parameter = 1; //or % maybe?
                }
            } else {
                continue;
            }
        }
    }
}