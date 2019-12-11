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
#include "lcmtypes/action_t.h"
#include "lcmtypes/world_t.h"
#include "lcmtypes/agent_t.h"
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

double constrain_lf(double val, double LL, double UL) {
    val = (val < LL) ? LL : val;
    val = (val > UL) ? UL : val;
    return val;
}

void publish_rate(state_t *state, double start_time) {
    int seconds = 0;
    long nanoseconds = state->delay_time * 1000 * 1000;
    nanoseconds -= (long)((seconds_now() - start_time) * pow(10, 9));
    struct timespec interval = {seconds, nanoseconds};
    nanosleep(&interval, NULL);
}

void print_interface(state_t *state) {
    int parameter = state->current_parameter;
    printf("\r");
    printf("%s%8.2d%s ", (parameter == 1) ? HIGHLIGHT : "",
           state->initial_runner_idx, CLEAR_HIGHLIGHT);
    printf("%s%8.2d%s ", (parameter == 2) ? HIGHLIGHT : "",
           state->delay_every, CLEAR_HIGHLIGHT);
    printf("%s%8.2f%s ", (parameter == 3) ? HIGHLIGHT : "",
           state->to_goal_magnitude, CLEAR_HIGHLIGHT);
    printf("%s%8.2d%s ", (parameter == 4) ? HIGHLIGHT : "",
           state->to_goal_power, CLEAR_HIGHLIGHT);
    printf("%s%8.2f%s ", (parameter == 5) ? HIGHLIGHT : "",
           state->avoid_obs_magnitude, CLEAR_HIGHLIGHT);
    printf("%s%8.2d%s ", (parameter == 6) ? HIGHLIGHT : "",
           state->avoid_obs_power, CLEAR_HIGHLIGHT);
    printf("%s%8.2d%s", (parameter == 7) ? HIGHLIGHT : "",
           state->max_velocity, CLEAR_HIGHLIGHT);
    fflush(stdout);
}

void robot_init(state_t *state) {
    state->chaser_message.theta = 0;
    state->chaser_message.x = (double)WIDTH / 2;
    state->chaser_message.y = (double)HEIGHT / 2;
    state->settings_message.initial_runner_idx = 17;
    state->settings_message.delay_every = 1;
    state->settings_message.to_goal_magnitude = 50.0;
    state->settings_message.to_goal_power = 0;
    state->settings_message.avoid_obs_magnitude = 1.0;
    state->settings_message.avoid_obs_power = -2;
    state->settings_message.max_vel = 12;
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

    state->chaser_message.vel = fmin(msg->vel, state->chaser_message.vel + 2.0);
    state->chaser_message.vel = fmin(state->chaser_message.vel, 12);

    state->chaser_message.ang_vel = constrain_lf(msg->ang_vel, -M_PI / 16, M_PI / 16);
}

int main(void) {
    bitmap_t bmp = {0};
    bmp_init(&bmp);

    state_t state = {0};
    state.lcm = lcm_create(NULL);
    robot_init(&state);
    //init_values(&state);
    state.delay_time = SLEEP_40 / state.delay_every;

    settings_t_subscribe(state.lcm, "SETTINGS_abusa", on_settings_t, &state.settings_message);
    reset_t_subscribe(state.lcm, "RESET_abusa", on_reset_t, &state.reset_message);
    action_t_subscribe(state.lcm, "ACTION_abusa", on_action_t, &state.action_message);

    // pthread_t chaser_thread;
    // pthread_create(&chaser_thread, NULL, io_thread, &state);
    // image_server_start("8000");

    state.timestep = 0;
    while (true) {
        lcm_handle_async(state.lcm);
        double start_time = seconds_now();

        chaser_moves(&state);
        runner_walks(&state);
        if (resolve_tile_collision(&state.chaser)) {
            state.chaser.fwd_vel *= 0.25;
        }
        if (resolve_tile_collision(&state.runner)) {
            state.runner.fwd_vel *= 0.25;
        }
        if (robots_collision(&state.chaser, &state.runner)) {
            printf("\e[2K\rRunner caught on step %d\n", state.timestep);
            reset_simulation(&state); //when chaser catches runner
            continue;
        }
        print_interface(&state);
        gx_draw_game(&bmp, &state); //update gx
        serving_img(bmp, &state); //delay 40ms and all

        world_t world_message;
        world_message.chaser = state.chaser_message;
        world_message.runner = state.runner_message;
        world_t_publish(state.lcm, "WORLD_abusa", &world_message);
        publish_rate(&state, start_time);
        state.timestep++;
    }
    free(bmp.data);
    // action_t_unsubscribe(state.lcm, action_sub);
    // reset_t_unsubscribe(state.lcm, reset_sub);
    // settings_t_unsubscribe(state.lcm, settings_sub);
    lcm_destroy(state.lcm);
    return 0;
}
