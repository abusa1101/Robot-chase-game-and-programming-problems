#define _GNU_SOURCE // this must come before all the #include's in order to work
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
#define M_PI 3.14159265358979323846
#define WIDTH 640
#define HEIGHT 480
#define BLOCK_SIZE 40
#define MAP "XXXXXXXXXXXXXXXX" \
            "X              X" \
            "X  XXXX   XXX  X" \
            "X   XX      X  X" \
            "X       XXX    X" \
            "XXXXXX         X" \
            "X         XXXXXX" \
            "X    XXX       X" \
            "X  XX     XX   X" \
            "X   X    XX    X" \
            "X      XXX     X" \
            "XXXXXXXXXXXXXXXX"
#define MAP_W (WIDTH / BLOCK_SIZE)
#define MAP_H (HEIGHT / BLOCK_SIZE)

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Error: Wrong number of arguments\n");
        return 1;
    }
    int timesteps = atoi(argv[1]);
    int speed = atoi(argv[2]);
    int run_index = atoi(argv[3]);
    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    image_server_start("8000");



    for (int t = 0; t < timesteps; t++) {

        //activateMove(&game);
        // for (int i = 0; i < game.n_lamp; i++) {
        //     resolve_collision(&game, game.lpos[i].x, game.lpos[i].y);
        // }
        if (speed == 0) {
            //gx_update(&bmp, &game, color_back, color_lamp, color_robot);
            size_t bmp_size = bmp_calculate_size(&bmp);
            uint8_t *serialized_bmp = malloc(bmp_size);
            bmp_serialize(&bmp, serialized_bmp);
            image_server_set_data(bmp_size, serialized_bmp);
            free(serialized_bmp);
            int seconds = 0;
            long nanoseconds = 40 * 1000 * 1000;
            struct timespec interval = {seconds, nanoseconds};
            nanosleep(&interval, NULL);
        }
    }


    //gx_update(&bmp, &game, color_back, color_lamp, color_robot);
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(&bmp, serialized_bmp);
    image_server_set_data(bmp_size, serialized_bmp);
    free(serialized_bmp);
    sleep(1);

    vector_free(lamp1);
    vector_free(lamp2);
    vector_free(lamp3);
    vector_free(robot);
    free(bmp.data);
    return 0;
}
