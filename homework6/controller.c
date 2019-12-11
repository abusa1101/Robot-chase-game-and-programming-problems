#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <lcm/lcm.h>
#include <time.h>
#include <pthread.h>
#include "image_server.h"
#include "bmp.h"
#include "all_functions.h"
#include "lcmtypes/settings_t.h"
#include "lcmtypes/reset_t.h"
#include "lcm_handle_async.h"

double seconds_now(void) {
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now)) {
        fprintf(stderr, "Retrieving system time failed.\n");
        exit(1);
    }
    return now.tv_sec + now.tv_nsec / 1000000000.0;
}

void publish_rate(state_t *state, double start_time) {
    int seconds = 0;
    long nanoseconds = (SLEEP_40 / state->delay_every) * 1000 * 1000;
    nanoseconds -= (long)((seconds_now() - start_time) * pow(10, 9));
    struct timespec interval = {seconds, nanoseconds};
    nanosleep(&interval, NULL);
}

void robot_init(state_t *state) {
    state->chaser.theta = 0;
    state->chaser.is_runner = false;
    state->runner.is_runner = true;
    state->initial_runner_idx = 17;
    state->delay_every = 1;
    state->to_goal_magnitude = 50.0;
    state->to_goal_power = 0;
    state->avoid_obs_magnitude = 1.0;
    state->avoid_obs_power = -2;
    state->max_vel = 12;
}

void on_settings_t(const lcm_recv_buf_t *rbuf, const char *channel,
                   const settings_t *msg, void *userdata) {
    state_t *state = userdata;
    state->delay_every = msg->delay_every;
}

void on_world_t(const lcm_recv_buf_t *rbuf, const char *channel,
                const world_t *msg, void *userdata) {
    state_t *state = userdata;
    state->chaser = msg->chaser;
    state->runner = msg->runner;

    potential_field_control(state);
    
    action_t action_message;
    action_message.vel = state.chaser.vel;
    action_message.ang_vel = state.chaser.ang_vel;
    action_t_publish(state.lcm, "ACTION_abusa", &action_message);
}

int main(int argc, char **argv) {
    state_t state = {0};
    state.lcm = lcm_create(NULL);
    init_values(&state);
    robot_init(&state);

    settings_t_subscribe(state.lcm, "SETTINGS_abusa", on_settings_t, &state);
    world_t_subscribe(state.lcm, "WORLD_abusa", on_world_t, &state);

    state.timestep = 0;
    while (true) {
        lcm_handle(state.lcm);
        double start_time = seconds_now();

        // chaser_moves(&state);
        // runner_walks(&state);
        // if (resolve_tile_collision(&state.chaser)) {
        //     state.chaser.vel *= 0.25;
        // }
        // if (resolve_tile_collision(&state.runner)) {
        //     state.runner.vel *= 0.25;
        // }
        // if (robots_collision(&state.chaser, &state.runner)) {
        //     printf("\e[2K\rRunner caught on step %d\n", state.timestep);
        //     reset_simulation(&state); //when chaser catches runner
        //     continue;
        // }

        // action_t action_message;
        // action_message.vel = state.chaser.vel;
        // action_message.ang_vel = state.chaser.ang_vel;
        // action_t_publish(state.lcm, "ACTION_abusa", &action_message);
        publish_rate(&state, start_time);

        state.timestep++;
    }
    lcm_destroy(state.lcm);
    return 0;
}
