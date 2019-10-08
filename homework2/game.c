#define _GNU_SOURCE // this must come before all the #include's in order to work
#include <unistd.h>
#include <time.h>
#include "image_server.h"
#include "bmp.h"
#include "gx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846

int main(void) {
    bitmap_t bmp = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    image_server_start("8000");

    color_bgr_t color_robot = {0, 255, 0};
    color_bgr_t color_back = {255, 255, 255};
    color_bgr_t color_lamp = {0, 255, 255};

    game_t game = {0};
    polygon_t rob = {0};
    polygon_t lamp1 ={0};
    polygon_t lamp2 ={0};
    polygon_t lamp3 ={0};

    init_values(&game);
    gx_update(&bmp, &game, color_back, color_lamp, color_robot);

    int timesteps = 200;
    for (int i = 0; i < timesteps; i++) { //Lamp1
        activateMove(&game);
        // check4collision(robot, lamp);
        // retreat(robot, lamp); //repeat till collision resolved
        gx_update(&bmp, &game, color_back, color_lamp, color_robot); //clear existing and create new


        //Image serializing & game sleep settings
        size_t bmp_size = bmp_calculate_size(&bmp);
        uint8_t *serialized_bmp = malloc(bmp_size);
        bmp_serialize(&bmp, serialized_bmp);
        // FILE *f = fopen("my_image3.bmp", "wb");
        // fwrite(serialized_bmp, bmp_size, 1, f);
        // fclose(f);
        image_server_set_data(bmp_size, serialized_bmp);


        //Update sleep settings
        //sleep(1);
        free(serialized_bmp);


        int seconds = 0;
        long nanoseconds = 40 * 1000 * 1000;
        struct timespec interval = { seconds, nanoseconds };
        nanosleep(&interval, NULL);
    }
    free(bmp.data);
    return 0;
}
