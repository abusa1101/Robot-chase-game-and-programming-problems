#include "bmp.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  // if (argc != 2) {
  //     fprintf(stderr, "Error: Wrong number of arguments\n");
  //     return 1;
  // }
  // int testnum = atoi(argv[1]);

  bitmap_t bmp = { 0 }; // initialize to zeros
  bmp.width = 640;
  bmp.height = 480;
  bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
  int color_value = 255;
  color_bgr_t color = {color_value, color_value, color_value};

  bresenham(10,10,200,200, &bmp, color);
  double xc = 0.0;
  double yc = 0.0;


  //rectangle = *give_rect(10.0, 20.0, xc, yc);


  size_t bmp_size = bmp_calculate_size(&bmp);
  uint8_t *serialized_bmp = malloc(bmp_size);
  bmp_serialize(&bmp, serialized_bmp);
  FILE *f = fopen("my_image.bmp", "wb");
  fwrite(serialized_bmp, bmp_size, 1, f);

  fclose(f);

  free(serialized_bmp);
  free(bmp.data);
  return 0;
}
