#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "image_server.h"
#include "bmp.h"
#include "all_functions.h"

int main(int argc, char **argv) {
    bitmap_t bmp = {0};
    state_t state = {0};
    init_values(&bmp, &state);
    pthread_t chaser_thread;
    pthread_create(&chaser_thread, NULL, io_thread, &state);

    gx_draw_game(&bmp, &state);
    while (true) {
        image_server_start("8000");
        chaser_movement(&state);
        gx_draw_game(&bmp, &state);
        serving_img(bmp);
        printf("%.2f %.2f\n", state.chaser.fwd_vel, state.chaser.ang_vel);
    }

    free(bmp.data);
    return 0;
}
