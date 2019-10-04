#include <unistd.h>
#include "image_server.h"
#include "bmp.h"
#include "vectors.h"
#include "test.h"
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

  bitmap_t bmp = { 0 }; // initialize to zeros
  bmp.width = 640;
  bmp.height = 480;
  bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
  int color_value = 255;
  color_bgr_t color = {color_value, color_value, color_value};
  pg_vector_t rect_vec = {0};
  pg_create(&rect_vec);
  pg_vector_t transformed_vec = {0};
  pg_create(&transformed_vec);

  if (input == 2) {
     bresenham(10,10,200,200, &bmp, color);
   } else if (input == 3) {
       double xc = 0.0;
       double yc = 0.0;
       give_rect(&rect_vec, 4.0, 4.0, xc, yc);
       cd2pixel(&rect_vec);
       pg_draw(&bmp, color, &rect_vec, 3);
   } else if (input == 4) {
     double xc = 2.0;
       double yc = 2.0;
       give_rect(&rect_vec, 4.0, 4.0, xc, yc);
       cd2pixel(&rect_vec);
       pg_draw(&bmp, color, &rect_vec, 0);
   } else if (input == 5) {
     double xc = 2.0;
       double yc = 2.0;
       give_rect(&rect_vec, 5.0, 5.0, xc, yc);
       cd2pixel(&rect_vec);
       pg_draw(&bmp, color, &rect_vec, 0);
  } else if (input == 6) {
    double xc = 320.0;
      double yc = 240.0;
      give_rect(&rect_vec, 600.0, 440.0, xc, yc);
      cd2pixel(&rect_vec);
      pg_draw(&bmp, color, &rect_vec, 0);
  } else if (input == 7) {
      double xc = 320.0;
      double yc = 240.0;
      give_rect(&rect_vec, 600.0, 440.0, xc, yc);
      cd2pixel(&rect_vec);
      pg_draw(&bmp, color, &rect_vec, 0);
      pg_fill(&bmp, color, &rect_vec);
   } else if (input == 8) {
     give_tri(&rect_vec, 21.0, 28.0, 400, 400);
     cd2pixel(&rect_vec);
     tri_draw(&bmp, color, &rect_vec);
     //tri_fill(&bmp, color, &rect_vec);
   } else if (input == 9) {
     give_tri(&rect_vec, 21.0, 28.0, 400, 400);
     cd2pixel(&rect_vec);
     rotate(&rect_vec, &transformed_vec, 30.0);
     tri_draw(&bmp, color, &transformed_vec);
     //tri_fill(&bmp, color, &rect_vec);
   }

  size_t bmp_size = bmp_calculate_size(&bmp);
  uint8_t *serialized_bmp = malloc(bmp_size);
  bmp_serialize(&bmp, serialized_bmp);
  FILE *f = fopen("my_image.bmp", "wb");
  fwrite(serialized_bmp, bmp_size, 1, f);
  fclose(f);

  image_server_set_data(bmp_size, serialized_bmp);
  image_server_start("8000"); // you could change the port number, but animation.html wants 8000
  sleep(1);
  pg_free(&transformed_vec);
  //pg_free(&tri_vec);
  pg_free(&rect_vec);
  free(serialized_bmp);
  free(bmp.data);
  return 0;
}
