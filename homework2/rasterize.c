#include "bmp.h"
#include <stdio.h>

int main(void) {
    bitmap_t bmp = { 0 }; // initialize to zeros
    bmp.width = 640;
    bmp.height = 480;
    // color_bgr_t represents the color for each pixel
    // calloc gives enough space for width*height of these pixels
    // and calloc also sets the initial values of all of these to zero (black)
    bmp.data = calloc(width * height, sizeof(color_bgr_t));

    // if we wanted to make the first pixel white
    // bmp.data[0].r = 255;
    // bmp.data[0].g = 255;
    // bmp.data[0].b = 255;

    // remember to later free the bmp.data when we are done using it
    free(bmp.data);

    return 0;

    bitmap_t bmp = { 0 };
    // set bmp width and height
    // allocate bytes for bmp.data
    // set pixels in bmp.data, for example:
    bmp.data[0].r = 255;

    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(&bmp, serialized_bmp);
    // serialized_bmp now has the full bmp formatted image

    // write to a file so we can check if it is a valid image
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    // free stuff we malloced

    printf("hello world!");

    return 0;
}
