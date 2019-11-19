#define _GNU_SOURCE
#include <unistd.h>
#include <time.h>
#include "image_server.h"
#include "bmp.h"
#include "all_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Error: Wrong number of arguments\n");
        return 1;
    }
    int timesteps = atoi(argv[1]);
    int speed = atoi(argv[2]);
    int run_index = atoi(argv[3]);
    bitmap_t bmp = {0};
    state_t state = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    if (speed != 2) {
        image_server_start("8000");
    }

    init_values(&state);
    gx_draw_game(&bmp, &state, run_index);

    for (int t = 0; t < timesteps; t++) {
        if (speed == 0) {
            play_game(&state);
            gx_draw_game(&bmp, &state, -1);

            size_t bmp_size = bmp_calculate_size(&bmp);
            uint8_t *serialized_bmp = malloc(bmp_size);
            bmp_serialize(&bmp, serialized_bmp);
            image_server_set_data(bmp_size, serialized_bmp);
            free(serialized_bmp);
            int seconds = 0;
            long nanoseconds = 40 * 1000 * 1000;
            struct timespec interval = {seconds, nanoseconds};
            nanosleep(&interval, NULL);
        } else if (speed == 1) {
            play_game(&state);

            size_t bmp_size = bmp_calculate_size(&bmp);
            uint8_t *serialized_bmp = malloc(bmp_size);
            bmp_serialize(&bmp, serialized_bmp);
            image_server_set_data(bmp_size, serialized_bmp);
            free(serialized_bmp);
            sleep(1);
        } else if (speed == 2) {
            play_game(&state);
        }

        if (robots_collision(&state.chaser, &state.runner)) {
            break;
        }
    }

    free(bmp.data);
    return 0;
}
