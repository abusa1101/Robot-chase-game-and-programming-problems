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
    if (argc != 3) {
        fprintf(stderr, "Error: Wrong number of arguments\n");
        return 1;
    }
    int timesteps = atoi(argv[1]);
    int fast = atoi(argv[2]);
    printf("%d,%d\n", timesteps, fast); //for no reason at all
    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));

    //Set or initialize constants
    color_bgr_t color_robot = {0, 255, 0};
    color_bgr_t color = {255, 255, 255};
    color_bgr_t color_lamp = {0, 255, 255};
    robot_t robot = {0};
    lamp_t lamp1 = {0};
    lamp_t lamp2 = {0};
    lamp_t lamp3 = {0};




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

//Update movement
// for (int i = 0; i < timesteps; i++) { //Lamp1
//   activateMove(&game);
//   check4collision(robot, lamp);
//   retreat(robot, lamp); //repeat till collision resolved
//   updateGraphics(game); //clear existing and create new
// }
