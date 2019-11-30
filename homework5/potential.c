#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "image_server.h"
#include "bmp.h"
#include "all_functions.h"

#define HIGHLIGHT "\e[7m"
#define CLEAR_HIGHLIGHT "\e[0m"

int main(int argc, char **argv) {
    bitmap_t bmp = {0};
    bmp_init(&bmp);

    state_t state = {0};
    init_values(&state);
    //reset_simulation(&state); //when it first runs

    pthread_t chaser_thread;
    pthread_create(&chaser_thread, NULL, io_thread, &state);

    image_server_start("8000");
    gx_draw_game(&bmp, &state);
    serving_img(bmp, &state); //delay 40ms and all

    while (true) {
        chaser_moves(&state);
        runner_walks(&state, &state.runner);
        if (robots_collision(&state.chaser, &state.runner)) {
            reset_simulation(&state); //when chaser catches runner
        }
        gx_draw_game(&bmp, &state); //update gx
        serving_img(bmp, &state); //delay 40ms and all

        printf("\r");
        printf("%s%d%s", HIGHLIGHT, state.initial_runner_idx, CLEAR_HIGHLIGHT);
        printf("%s%d%s", HIGHLIGHT, state.delay_every, CLEAR_HIGHLIGHT);
        printf("%s%8.2f%s", HIGHLIGHT, state.to_goal_magnitude, CLEAR_HIGHLIGHT);
        printf("%s%d%s", HIGHLIGHT, state.to_goal_power, CLEAR_HIGHLIGHT);
        printf("%s%8.2f%s", HIGHLIGHT, state.avoid_obs_magnitude, CLEAR_HIGHLIGHT);
        printf("%s%d%s", HIGHLIGHT, state.avoid_obs_power, CLEAR_HIGHLIGHT);
        printf("%s%d%s", HIGHLIGHT, state.max_velocity, CLEAR_HIGHLIGHT);
        fflush(stdout);
        printf("\e[?25h");
    }
    free(bmp.data);
    return 0;
}
