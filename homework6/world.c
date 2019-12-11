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

#define HIGHLIGHT "\e[7m"
#define CLEAR_HIGHLIGHT "\e[0m"

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
    long nanoseconds = (SLEEP_40 / state->settings_message.delay_every) * 1000 * 1000;
    nanoseconds -= (long)((seconds_now() - start_time) * pow(10, 9));
    struct timespec interval = {seconds, nanoseconds};
    nanosleep(&interval, NULL);
}

void print_interface(state_t *state) {
    int parameter = state->current_parameter;
    printf("\r");
    printf("%s%8.2d%s ", (parameter == 1) ? HIGHLIGHT : "",
           state->settings_message.initial_runner_idx, CLEAR_HIGHLIGHT);
    printf("%s%8.2d%s ", (parameter == 2) ? HIGHLIGHT : "",
           state->settings_message.delay_every, CLEAR_HIGHLIGHT);
    printf("%s%8.2f%s ", (parameter == 3) ? HIGHLIGHT : "",
           state->settings_message.to_goal_magnitude, CLEAR_HIGHLIGHT);
    printf("%s%8.2d%s ", (parameter == 4) ? HIGHLIGHT : "",
           state->settings_message.to_goal_power, CLEAR_HIGHLIGHT);
    printf("%s%8.2f%s ", (parameter == 5) ? HIGHLIGHT : "",
           state->settings_message.avoid_obs_magnitude, CLEAR_HIGHLIGHT);
    printf("%s%8.2d%s ", (parameter == 6) ? HIGHLIGHT : "",
           state->settings_message.avoid_obs_power, CLEAR_HIGHLIGHT);
    printf("%s%8.2f%s", (parameter == 7) ? HIGHLIGHT : "",
           state->settings_message.max_vel, CLEAR_HIGHLIGHT);
    fflush(stdout);
}

void robot_init(state_t *state) {
    state->chaser.theta = 0;
    //state->runner.theta = 0;
    state->chaser.is_runner = false;
    state->runner.is_runner = true;
    state->chaser.x = (double)WIDTH / 2;
    state->chaser.y = (double)HEIGHT / 2;
    state->settings_message.initial_runner_idx = 17;
    give_runner_pos(state, state->settings_message.initial_runner_idx);
    state->settings_message.delay_every = 1;
    state->settings_message.to_goal_magnitude = 50.0;
    state->settings_message.to_goal_power = 0;
    state->settings_message.avoid_obs_magnitude = 1.0;
    state->settings_message.avoid_obs_power = -2;
    state->settings_message.max_vel = 12;
    // boolean is_runner;
    // double x;
    // double y;
    // double theta;
    // double vel;
    // double ang_vel;
}

void on_settings_t(const lcm_recv_buf_t *rbuf, const char *channel,
                   const settings_t *msg, void *userdata) {
    state_t *state = userdata;
    state->settings_message.delay_every = msg->delay_every;
}

void on_reset_t(const lcm_recv_buf_t *rbuf, const char *channel,
                const reset_t *msg, void *userdata) {
    state_t *state = userdata;
    state->reset_message.initial_runner_idx = msg->initial_runner_idx;
}

void on_action_t(const lcm_recv_buf_t *rbuf, const char *channel,
                 const action_t *msg, void *userdata) {
    state_t *state = userdata;

    state->chaser.vel = fmin(msg->vel, state->chaser.vel + 2.0);
    state->chaser.vel = fmin(state->chaser.vel, 12);

    double ang_vel = msg->ang_vel;
    ang_vel = (ang_vel < -M_PI / 16) ? -M_PI / 16 : ang_vel;
    ang_vel = (ang_vel > M_PI / 16) ? M_PI / 16 : ang_vel;
    state->chaser.ang_vel = ang_vel;
    //state->chaser_message.ang_vel = constrain_lf(msg->ang_vel, -M_PI / 16, M_PI / 16);
}

int main(void) {
    bitmap_t bmp = {0};
    bmp_init(&bmp);

    state_t state = {0};
    state.lcm = lcm_create(NULL);
    //init_values(&state);
    robot_init(&state);

    settings_t_subscribe(state.lcm, "SETTINGS_abusa", on_settings_t, &state);
    reset_t_subscribe(state.lcm, "RESET_abusa", on_reset_t, &state);
    action_t_subscribe(state.lcm, "ACTION_abusa", on_action_t, &state);

    state.timestep = 0;
    while (true) {
        lcm_handle_async(state.lcm);
        double start_time = seconds_now();

        chaser_moves(&state);
        runner_walks(&state);
        if (resolve_tile_collision(&state.chaser)) {
            state.chaser.vel *= 0.25;
        }
        if (resolve_tile_collision(&state.runner)) {
            state.runner.vel *= 0.25;
        }
        if (robots_collision(&state.chaser, &state.runner)) {
            printf("\e[2K\rRunner caught on step %d\n", state.timestep);
            reset_simulation(&state); //when chaser catches runner
            continue;
        }
        print_interface(&state);
        state.settings_message.initial_runner_idx = state.initial_runner_idx;
        state.settings_message.delay_every = state.delay_every;
        state.settings_message.to_goal_magnitude = state.to_goal_magnitude;
        state.settings_message.to_goal_power = state.to_goal_power;
        state.settings_message.avoid_obs_magnitude = state.avoid_obs_magnitude;
        state.settings_message.avoid_obs_power = state.avoid_obs_power;
        state.settings_message.max_vel = state.max_vel;
        gx_draw_game(&bmp, &state); //update gx
        serving_img(bmp, &state); //delay 40ms and all

        world_t world_message;
        world_message.chaser = state.chaser;
        world_message.runner = state.runner;
        world_t_publish(state.lcm, "WORLD_abusa", &world_message);
        publish_rate(&state, start_time);

        state.timestep++;
    }
    free(bmp.data);
    lcm_destroy(state.lcm);
    return 0;
}
