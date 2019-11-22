#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "image_server.h"
#include "bmp.h"
#include "all_functions.h"

int main(int argc, char **argv) {
    bitmap_t bmp = {0};
    bmp_init(&bmp);

    state_t state = {0};
    init_values(&state);
    
    pthread_t chaser_thread;
    pthread_create(&chaser_thread, NULL, io_thread, &state);

    gx_draw_game(&bmp, &state);
    for (int t = 0; t < state.delay_every; t++) {
        image_server_start("8000");

        play_game(&state);

        gx_draw_game(&bmp, &state); //update gx
        serving_img(bmp); //delay 40ms and all

        printf("%.2f %.2f\n", state.chaser.fwd_vel, state.chaser.ang_vel);
        if (robots_collision(&state.chaser, &state.runner)) {
            break;
        }
    }

    free(bmp.data);
    return 0;
}
