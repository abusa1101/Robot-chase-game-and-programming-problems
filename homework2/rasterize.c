#include <unistd.h>
#include "image_server.h"
#include "bmp.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Error: Wrong number of arguments\n");
        return 1;
    }
    int input = atoi(argv[1]);

    bitmap_t bmp = {0}; // initialize to zeros
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    int color_value = 255;
    color_bgr_t color = {color_value, color_value, color_value};

    pg_vector_t rect_vec = {0};
    pg_create(&rect_vec);
    pg_vector_t transformed_vec = {0};
    pg_create(&transformed_vec);
    pg_vector_t fillbuff = {0};
    pg_create2(&fillbuff);

    CallFunctions(bmp, color, input, rect_vec, transformed_vec, fillbuff);

    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(&bmp, serialized_bmp);
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000"); // you could change the port number, but animation.html wants 8000
    sleep(1);
    pg_free(&fillbuff);
    pg_free(&transformed_vec);
    pg_free(&rect_vec);
    free(serialized_bmp);
    free(bmp.data);
    return 0;
}
