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

void publish_rate(double start_time) {
    int seconds = 0;
    long nanoseconds = SLEEP_40 * 1000 * 1000;
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

void on_world_t(const lcm_recv_buf_t *rbuf, const char *channel,
            const world_t *msg, void *userdata) {
    world_t *msg = userdata;
    //printf("%.2f %.2f %.2f\n", lcm_message->l2g[0], lcm_message->l2g[1], lcm_message->l2g[2]);
}

void on_settings_t(const lcm_recv_buf_t *rbuf, const char *channel,
            const settings_t *msg, void *userdata) {
    settings_t *msg = userdata;
    //printf("%.2f %.2f %.2f\n", lcm_message->l2g[0], lcm_message->l2g[1], lcm_message->l2g[2]);
}

void on_reset_t(const lcm_recv_buf_t *rbuf, const char *channel,
            const reset_t *msg, void *userdata) {
    reset_t *msg = userdata;
    //printf("%.2f %.2f %.2f\n", lcm_message->l2g[0], lcm_message->l2g[1], lcm_message->l2g[2]);
}

void on_agent_t(const lcm_recv_buf_t *rbuf, const char *channel,
            const agent_t *msg, void *userdata) {
    agent_t *msg = userdata;
    //printf("%.2f %.2f %.2f\n", lcm_message->l2g[0], lcm_message->l2g[1], lcm_message->l2g[2]);
}

int main(void) {
    bitmap_t bmp = {0};
    bmp_init(&bmp);

    state_t state = {0};
    state.lcm = lcm_create(NULL);
    init_values(&state);

    world_t_subscription_t *world_sub = world_t_subscribe(state.lcm, "WORLD_abusa", on_world_t, &state.world_message);
    settings_t_subscription_t *settings_sub = settings_t_subscribe(state.lcm, "SETTINGS_abusa", on_settings_t, &state.settings_message);
    reset_t_subscription_t *reset_sub = reset_t_subscribe(state.lcm, "RESET_abusa", on_reset_t, &state.reset_message);
    agent_t_subscription_t *agent_sub = agent_t_subscribe(state.lcm, "AGENT_abusa", on_agent_t, &state.agent_message);

    pthread_t chaser_thread;
    pthread_create(&chaser_thread, NULL, io_thread, &state);
    image_server_start("8000");
    //gx_draw_game(&bmp, &state);
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
            reset_simulation(&state); //when chaser catches runner
            continue;
        }
        print_interface(&state);

        state_message.initial_runner_idx = state.initial_runner_idx;
        state_message.delay_every = state.delay_every;
        state_message.to_goal_magnitude = state.to_goal_magnitude;
        state_message.to_goal_power = state.to_goal_power;
        state_message.avoid_obs_magnitude = state.avoid_obs_magnitude;
        state_message.avoid_obs_power = state.avoid_obs_power;
        state_message.max_velocity = state.max_velocity;

        gx_draw_game(&bmp, &state); //update gx
        serving_img(bmp, &state); //delay 40ms and all
        settings_t_publish(state.lcm, "SETTINGS_abusa", &state.state_message);
        publish_rate(start_time);
    }
    free(bmp.data);
    agent_t_unsubscribe(state.lcm, agent_sub);
    reset_t_unsubscribe(state.lcm, reset_sub);
    settings_t_unsubscribe(state.lcm, settings_sub);
    world_t_unsubscribe(state.lcm, world_sub);
    lcm_destroy(state.lcm);
    return 0;
}
