#define _GNU_SOURCE // this must come before all the #include's in order to work
#include <unistd.h>
#include <time.h>
#include "image_server.h"
#include "bmp.h"
#include "gx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define M_PI 3.14159265358979323846

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Error: Wrong number of arguments\n");
        return 1;
    }
    int timesteps = atoi(argv[1]);
    int speed = atoi(argv[2]);
    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    image_server_start("8000");

    color_bgr_t color_robot = {0, 255, 0};
    color_bgr_t color_back = {255, 255, 255};
    color_bgr_t color_lamp = {0, 255, 255};

    game_t game = {0};
    vector_xy_t *robot = vector_create();
    vector_xy_t *lamp1 = vector_create();
    vector_xy_t *lamp2 = vector_create();
    vector_xy_t *lamp3 = vector_create();

    init_values(&game);

    for (int t = 0; t < timesteps; t++) {
        //if (t == 0) {
            gx_update(&bmp, &game, color_back, color_lamp, color_robot);
        //} else {
            activateMove(&game);
            for (int i = 0; i < game.n_lamp; i++) {
                resolve_collision(&game, game.lpos[i].x, game.lpos[i].y);
            }
            if (speed == 0) {
                gx_update(&bmp, &game, color_back, color_lamp, color_robot);
                size_t bmp_size = bmp_calculate_size(&bmp);
                uint8_t *serialized_bmp = malloc(bmp_size);
                bmp_serialize(&bmp, serialized_bmp);
                image_server_set_data(bmp_size, serialized_bmp);
                free(serialized_bmp);
                int seconds = 0;
                long nanoseconds = 40 * 1000 * 1000;
                struct timespec interval = { seconds, nanoseconds };
                nanosleep(&interval, NULL);
            }
        //}
    }
    gx_update(&bmp, &game, color_back, color_lamp, color_robot);
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

// printf("%f, %f\n", lamp1.c_x, lamp1.c_y);
//     for (int i = 0; i < 3; i++) {
//         printf("%f, %f\n", lamp1.x[i], lamp1.y[i]);
//     }
//     printf("\n");
//     printf("%f, %f\n", lamp2.c_x, lamp2.c_y);
//     for (int i = 0; i < 3; i++) {
//         printf("%f, %f\n", lamp2.x[i], lamp2.y[i]);
//     }
//     printf("\n");
//     printf("%f, %f\n", lamp3.c_x, lamp3.c_y);
//     for (int i = 0; i < 3; i++) {
//         printf("%f, %f\n", lamp3.x[i], lamp3.y[i]);
//     }
// printf("\n");
