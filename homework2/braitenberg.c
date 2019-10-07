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

  //Initialize and set black image
  bitmap_t bmp = {0}; // initialize to zeros
  bmp.width = 640;
  bmp.height = 480;
  bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));

  //Set up my game objects and vectors
  // game_t game = {0};
  // robot_t robot = {0};
  // lamp_t lamp1 = {0};
  // lamp_t lamp2 = {0};
  // lamp_t lamp3 = {0};

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
  //setConstants(robot, lamp1, lamp2, lamp3);
  color_bgr_t color_robot = {0, 255, 0};
  color_bgr_t color = {255, 255, 255};
  color_bgr_t color_lamp = {0, 255, 255};

   //border
   double xcb = 320.0;
   double ycb = 240.0;
   give_rect(&rect_vec, 600.0, 440.0, xcb, ycb);
   cd2pixel(&rect_vec);
   pg_draw(&bmp, color, &rect_vec, 0);

   //robot
   //printf("%f %f", robot.width, robot.length);
   give_tri(&trans4_vec, 21, 28, 320, 240);
   cd2pixel(&trans4_vec);
   tri_draw(&bmp, color_robot, &trans4_vec);
   callb2(&fillbuff4, &trans4_vec);
   tri_fill(&bmp, color_robot, &trans4_vec, &fillbuff4);

   //lamps
   double lamp_size = 12.0;
   double xcl1 = 124.1;
   double ycl1 = 224.1;
   give_rect(&l1_vec, lamp_size, lamp_size, xcl1, ycl1);
   rotate2(&l1_vec, &trans1_vec, 45, xcl1, ycl1);
   cd2pixel(&trans1_vec);
   pg_draw(&bmp, color_lamp, &trans1_vec, 0);
   callb2(&fillbuff1, &trans1_vec);
   tri_fill(&bmp, color_lamp, &trans1_vec, &fillbuff1);

   double xcl2 = 349.1;
   double ycl2 = 99.1;
   give_rect(&l2_vec, lamp_size, lamp_size, xcl2, ycl2);
   rotate2(&l2_vec, &trans2_vec, 45, xcl2, ycl2);
   cd2pixel(&trans2_vec);
   pg_draw(&bmp, color_lamp, &trans2_vec, 0);
   callb2(&fillbuff2, &trans2_vec);
   tri_fill(&bmp, color_lamp, &trans2_vec, &fillbuff2);

   double xcl3 = 449.1;
   double ycl3 = 349.1;
   give_rect(&l3_vec, lamp_size, lamp_size, xcl3, ycl3);
   rotate2(&l3_vec, &trans3_vec, 45, xcl3, ycl3);
   cd2pixel(&trans3_vec);
   pg_draw(&bmp, color_lamp, &trans3_vec, 0);
   callb2(&fillbuff3, &trans3_vec);
   tri_fill(&bmp, color_lamp, &trans3_vec, &fillbuff3);

  // //Update movement
  // for (int i = 0; i < timesteps; i++) { //Lamp1
  //   activateLamp(game, robot);
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
