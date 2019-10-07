#define _GNU_SOURCE // this must come before all the #include's in order to work
#include <unistd.h>
#include <time.h>
#include "image_server.h"
#include "bmp.h"
#include "movement.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846

int main(int argc, char **argv) {
    //Set and read input arguments
    if (argc != 3) {
        fprintf(stderr, "Error: Wrong number of arguments\n");
        return 1;
    }
    int timesteps = atoi(argv[1]);
    int fast = atoi(argv[2]);
    printf("%d,%d\n", timesteps, fast); //for no reason at all

    bitmap_t bmp = {0}; // initialize to zeros
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));

    pg_vector_t rect_vec = {0};
    pg_create2(&rect_vec);

    pg_vector_t trans1_vec = {0};
    pg_create2(&trans1_vec);
    pg_vector_t trans2_vec = {0};
    pg_create2(&trans2_vec);
    pg_vector_t trans3_vec = {0};
    pg_create2(&trans3_vec);
    pg_vector_t trans4_vec = {0};
    pg_create2(&trans4_vec);

    pg_vector_t l1_vec = {0};
    pg_create2(&l1_vec);
    pg_vector_t l2_vec = {0};
    pg_create2(&l2_vec);
    pg_vector_t l3_vec = {0};
    pg_create2(&l3_vec);

    pg_vector_t fillbuff1 = {0};
    pg_create2(&fillbuff1);
    pg_vector_t fillbuff2 = {0};
    pg_create2(&fillbuff2);
    pg_vector_t fillbuff3 = {0};
    pg_create2(&fillbuff3);
    pg_vector_t fillbuff4 = {0};
    pg_create2(&fillbuff4);

    //Set or initialize constants
    color_bgr_t color_robot = {0, 255, 0};
    color_bgr_t color = {255, 255, 255};
    color_bgr_t color_lamp = {0, 255, 255};
    robot_t robot = {0};
    lamp_t lamp1 = {0};
    lamp_t lamp2 = {0};
    lamp_t lamp3 = {0};

    initGraphics(bmp, color, color_robot, color_lamp, &robot, &lamp1, &lamp2, &lamp3,
                      rect_vec, trans1_vec, fillbuff1, trans2_vec, fillbuff2, trans3_vec,
                      fillbuff3, trans4_vec, fillbuff4);

    //Update movement
    // for (int i = 0; i < timesteps; i++) { //Lamp1
    //   activateMove(&game);
    //   check4collision(robot, lamp);
    //   retreat(robot, lamp); //repeat till collision resolved
    //   updateGraphics(game); //clear existing and create new
    // }

    //Image serializing & game sleep settings
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(&bmp, serialized_bmp);
    FILE *f = fopen("my_image2.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);
    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000");

    //Free vectors
    pg_free(&fillbuff4);
    pg_free(&fillbuff1);
    pg_free(&fillbuff2);
    pg_free(&fillbuff3);
    pg_free(&trans4_vec);
    pg_free(&rect_vec);
    pg_free(&l1_vec);
    pg_free(&l2_vec);
    pg_free(&l3_vec);
    pg_free(&trans1_vec);
    pg_free(&trans2_vec);
    pg_free(&trans3_vec);

    //Update sleep settings
    sleep(1);
    free(serialized_bmp);
    free(bmp.data);
    int seconds = 0;
    long nanoseconds = 40 * 1000 * 1000;
    struct timespec interval = { seconds, nanoseconds };
    nanosleep(&interval, NULL);

    return 0;
}
