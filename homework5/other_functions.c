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

int give_runner_pos2(double x, double y) { //FIX THIS- HOW TO GET IDX FROM X, Y
    int map_x = (x * MAP_W / WIDTH);
    int map_y = (y * MAP_H / HEIGHT);
    int pos = MAP[map_y * MAP_W + map_x] //?????
    return pos;
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
            reset_simulation();
        }
        if (c == '\e' && getc(stdin) == '[') {
            c = getc(stdin);
            if (c == 'A') { //up
                state->user_action = 1;
                update_parameter(state, 1);
            } else if (c == 'B') { //down
                state->user_action = 4;
                update_parameter(state, 0);
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
    state->avoid_obs_magnitude 1.0;
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
    // int action = state->user_action; //FIX THIS- WHAT TO FILL HERE, WHERE R WE CALLING THIS FUNC?
    // if (action == 1) { //up
    //     state->chaser.fwd_vel += 4;
    //     if (state->chaser.fwd_vel > state->max_velocity) {
    //         state->chaser.fwd_vel = state->max_velocity;
    //     }
    // } else if (action == 2) { //left
    //     state->chaser.ang_vel -= M_PI / 32;
    // } else if (action == 3) { //right
    //     state->chaser.ang_vel += M_PI / 32;
    // }
    state->chaser.theta += state->chaser.ang_vel;
    state->chaser.ang_vel *= 0.8;
    move(&state->chaser);
    if (resolve_tile_collision(&state->chaser)) {
        state->chaser.fwd_vel *= 0.25;
    }
    //state->user_action = 0;
}

void play_game(state_t *state) {
    runner_walks(&state->runner);
    chaser_movement(state);
}


//Potential Field
void potential_field_control(state_t *state) {
    double robot_r = sqrt((ROB_W / 2)^2 + (ROB_L / 2)^2); //circle approx radius of robot
    double wall_r = BLOCK_SIZE / sqrt(2); //circle approx radius of wall block
    double fx = 0.0; //forces on robot
    double fy = 0.0;

    double dist_sq_robots = pow(state->runner.x - state->chaser.x, 2) +
                             pow(state->runner.y - state->chaser.y, 2);
    double to_goal_x = (state->runner.x - state->chaser.x) / sqrt(dist_sq_robots); //unit vec from chaser to runner
    double to_goal_y = (state->runner.y - state->chaser.y) / sqrt(dist_sq_robots);
    double to_goal_dist_x = state->runner.x - state->chaser.x; //dist from chaser to runner
    double to_goal_dist_y = state->runner.y - state->chaser.y;

    fx += to_goal_x * state->to_goal_magnitude * pow(to_goal_dist_x, state->to_goal_power);
    fy += to_goal_y * state->to_goal_magnitude * pow(to_goal_dist_y, state->to_goal_power);

    int map_x = (state->chaser.x * MAP_W / WIDTH);
    int map_y = (state->robot.y * MAP_H / HEIGHT);
    for (int y = (int)max(map_y - 1, 0); y <= map_y + 1; y++) {
        for (int x = (int)max(map_x - 1, 0); x <= map_x + 1; x++) {
            double dist_sq = pow(state->chaser.x - x, 2) + pow(state->chaser.y - y, 2);
            double to_chaser_x = (state->chaser.x - x) / sqrt(dist_sq); //unit vector from the wall block to the chaser
            double to_chaser_y = (state->chaser.y - y) / sqrt(dist_sq);
            double to_obs_dist = sqrt(dist_sq_robots) - (2 * robot_r); //dist between runner and chaser, approx them as circles
            to_obs_dist = fmax(0.1, to_obs_dist);
            fx += to_chaser_x * state->avoid_obs_magnitude * pow(to_obs_dist, state->avoid_obs_power);
            fy += to_chaser_y * state->avoid_obs_magnitude * pow(to_obs_dist, state->avoid_obs_power);
        }
    }

    double target_theta = acos(fx);//theta of fx, fy vector FIX THIS also rad/deg?
    double theta_error = target_theta - state->chaser.theta; //wrapped around to range [-pi, pi] FIX THIS WRAP AS IN?
    if (theta_error > M_PI) {
        theta_error = -M_PI + (theta_error - M_PI);
    } else if (theta_error < -M_PI) {
        theta_error = M_PI - (-theta_error - M_PI);
    }
    double ang_vel = 0.4 * theta_error; //constrained to range [-pi / 16, pi / 16]
    state->chaser.ang_velocity = constrain(ang_vel, - M_PI / 16, M_PI / 16);

    state->chaser.fwd_vel = fmin(max_velocity, state->chaser.fwd_vel + 2.0);
}

void update_parameters(state_t *state, bool action_is_up) {
    if (action_is_up) {
        if (state->current_parameter == 1) {
            int start_location = give_runner_pos2(state->runner.x, state->runner.y);
            state->initial_runner_idx = start_location;
        } else if (state->current_parameter == 2) {
            state->delay_every += 1;
            constrain(state->delay_every, 1, 10000000); //inf basically
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
            printf("UP Error: No such parameter (>7). Check IO_thread");
        }
    } else { //action = down
        if (state->current_parameter == 1) {
            state->initial_runner_idx = give_runner_pos2(state->runner.x, state->runner.y);
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
            printf("DOWN Error: No such parameter (<7). Check IO_thread");
        }
    }
}

void reset_simulation(state_t *state) {
    srand(0);
    init_values(state);

    // int map_x = (robot->x * MAP_W / WIDTH); //FIX THIS
    // int map_y = (robot->y * MAP_H / HEIGHT);
    // (int y = (int)map_y
    // (int x = (int)map_x

    // int x = state->runner.x; //FIX THIS
    // int y = state->runner.y;
    // int counter = 0;
    // while (MAP[y * MAP_W + x] == 'X') {
    //     counter++;
    // }
    // state->initial_runner_idx = give_runner_pos2(state->runner.x + counter, state->runner.y + counter);

    state->initial_runner_idx = give_runner_pos2(state->runner.x, state->runner.y); //FIX THIS

    state->runner.fwd_vel = 0;
    state->runner.ang_vel = 0;
    state->runner.theta = 0;

    state->chaser.fwd_vel = 0;
    state->chaser.ang_vel = 0;
    state->chaser.theta = 0;

    state->current_parameter = 1;
}
