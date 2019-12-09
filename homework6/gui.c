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

int main(void) {
    bitmap_t bmp = {0};
    bmp_init(&bmp);

    state_t state = {0};
    state.lcm = lcm_create(NULL);
    init_values(&state);

    settings_t state_message;
    
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
            //printf("\e[2K\rRunner caught on step %d\n", state.timestep);
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
    lcm_destroy(state.lcm);
    return 0;
}
