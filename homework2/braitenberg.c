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

// At each time step we take the following steps:
//     Update movement
//     Check for and completely resolve collisions
//     Update graphics
//     Sleep for 40ms to make a 25fps animation.

int main(int argc, char **argv) {
  //Set and read input arguments
  if (argc != 3) {
      fprintf(stderr, "Error: Wrong number of arguments\n");
      return 1;
  }
  int timesteps = atoi(argv[1]);
  int fast = atoi(argv[2]);

  //Initialize and set black image
  bitmap_t bmp = {0}; // initialize to zeros
  bmp.width = 640;
  bmp.height = 480;
  bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
  //Set up my game objects
  game_t game = {0};
  robot_t robot = {0};
  lamp_t lamp1 = {0};
  lamp_t lamp2 = {0};
  lamp_t lamp3 = {0};
  //Constants
  game.color = {255, 255, 255}; //color_bgr_t color_robot =
  robot.color = {0, 255, 0};
  lamp1.color = {0, 255, 255};
  lamp2.color = {0, 255, 255};
  lamp3.color = {0, 255, 255};
  robot.max_movement = 12; //max pixels mvmt in a timestep
  lamp1.lamp_power = 100000;
  lamp2.lamp_power = 100000;
  lamp3.lamp_power = 100000;
  lamp1.size = 12;
  lamp2.size = 12;
  lamp3.size = 12;
  lamp1.location.x = 124.1;
  lamp1.location.y = 224.1;
  lamp2.location.x = 349.1;
  lamp2.location.y = 99.1;
  lamp3.location.x = 449.1;
  lamp3.location.y = 349.1;

  //Variables
  robot.location.x = 320;
  robot.location.y = 240;
  //robot.move_l = 0;
  //robot.move_r = 0;
  robot.dir = {0};
  robot.wheel_base = 80;

  //Update movement
  for (int i = 0; i < timesteps; i++) { //Lamp1
    activateLamp(game, robot);
    //check4collision
    //move away - repeat till collision resolved
    //update graphics- clear existing and create new


}



  //Image serializing & game sleep settings
  size_t bmp_size = bmp_calculate_size(&bmp);
  uint8_t *serialized_bmp = malloc(bmp_size);
  bmp_serialize(&bmp, serialized_bmp);
  FILE *f = fopen("my_image2.bmp", "wb");
  fwrite(serialized_bmp, bmp_size, 1, f);
  fclose(f);
  image_server_set_data(bmp_size, serialized_bmp);
  image_server_start("8000"); // you could change the port number, but animation.html wants 8000
  sleep(1);
  free(serialized_bmp);
  free(bmp.data);
  int seconds = 0;
  long nanoseconds = 40 * 1000 * 1000;
  struct timespec interval = { seconds, nanoseconds };
  nanosleep(&interval, NULL);

  return 0;
}
