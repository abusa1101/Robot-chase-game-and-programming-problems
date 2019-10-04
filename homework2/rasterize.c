#include "bmp.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
  // bresenham(10,10,200,200, &bmp, color);
  double xc = 2.0;
  double yc = 2.0;
  pg_vector_t rect_vec = {0};
  pg_create(&rect_vec);
  give_rect(&rect_vec, 4.0, 4.0, xc, yc);
  // for (int i = 0; i <rect_vec.size; i++) {
  //     printf("%d ", rect_vec.data[i].x);
  //     printf("%d\n", rect_vec.data[i].y);
  //     //bmp.data[rect_vec.data[i].y * bmp.width + rect_vec.data[i].x] = color;
  //   }
  cd2pixel(&rect_vec);
  // for (int i = 0; i <rect_vec.size; i++) {
  //     printf("%d ", rect_vec.data[i].x);
  //     printf("%d\n", rect_vec.data[i].y);
  //     //bmp.data[rect_vec.data[i].y * bmp.width + rect_vec.data[i].x] = color;
  //   }
  // pg_vector_t transformed_vec = {0};
  // pg_create(&transformed_vec);
  // double xglobal = 50;
  // double yglobal = 25;
  // double radrot = 0;
  // translate(&rect_vec, &transformed_vec, xglobal, yglobal);
  // for (int i = 0; i <transformed_vec.size; i++) {
  //     printf("%d ", transformed_vec.data[i].x);
  //     printf("%d\n", transformed_vec.data[i].y);
  //   }
  pg_draw(&bmp, color, &rect_vec);
  //pg_draw(&bmp, color, &transformed_vec);

  size_t bmp_size = bmp_calculate_size(&bmp);
  uint8_t *serialized_bmp = malloc(bmp_size);
  bmp_serialize(&bmp, serialized_bmp);
  FILE *f = fopen("my_image.bmp", "wb");
  fwrite(serialized_bmp, bmp_size, 1, f);
  fclose(f);
  // pg_free(&transformed_vec);
   pg_free(&rect_vec);
  free(serialized_bmp);
  free(bmp.data);
  return 0;
}
