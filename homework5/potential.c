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
    state.timestep = 0;
    while (true) {
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
        if (state.timestep % state.delay_every == 0) {
            gx_draw_game(&bmp, &state); //update gx
            serving_img(bmp, &state); //delay 40ms and all
        }
        //printf("x,y = %lf, %lf |t = %lf |f,a = %lf, %lf\n", state.runner.x, state.runner.y, state.runner.theta, state.runner.fwd_vel, state.runner.ang_vel);
        int parameter = state.current_parameter;
        printf("\r");
        printf("%s%8.2d%s ", (parameter == 1) ? HIGHLIGHT : "",
               state.initial_runner_idx, CLEAR_HIGHLIGHT);
        printf("%s%8.2d%s ", (parameter == 2) ? HIGHLIGHT : "",
               state.delay_every, CLEAR_HIGHLIGHT);
        printf("%s%8.2f%s ", (parameter == 3) ? HIGHLIGHT : "",
               state.to_goal_magnitude, CLEAR_HIGHLIGHT);
        printf("%s%8.2d%s ", (parameter == 4) ? HIGHLIGHT : "",
               state.to_goal_power, CLEAR_HIGHLIGHT);
        printf("%s%8.2f%s ", (parameter == 5) ? HIGHLIGHT : "",
               state.avoid_obs_magnitude, CLEAR_HIGHLIGHT);
        printf("%s%8.2d%s ", (parameter == 6) ? HIGHLIGHT : "",
               state.avoid_obs_power, CLEAR_HIGHLIGHT);
        printf("%s%8.2d%s", (parameter == 7) ? HIGHLIGHT : "",
               state.max_velocity, CLEAR_HIGHLIGHT);
        fflush(stdout);

        state.timestep++;
    }
    free(bmp.data);
    return 0;
}
