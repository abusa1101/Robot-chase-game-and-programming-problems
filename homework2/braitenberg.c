#include <unistd.h>
#include <time.h>
#include "image_server.h"
#include "bmp.h"
#include "vectors.h"
#include "test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define _GNU_SOURCE // this must come before all the #include's in order to work
#define M_PI 3.14159265358979323846

int main(int argc, char **argv) {
  if (argc != 3) {
      fprintf(stderr, "Error: Wrong number of arguments\n");
      return 1;
  }
  int timesteps = atoi(argv[1]);
  int fast = atoi(argv[1]);

  printf("%d\n", timesteps);
  printf("%d\n", fast);

  move_l = 0
move_r = 0
for each lamp l:
    dist_sq = square distance from robot to lamp l
    dir = unit vector pointing from robot to lamp l
    eye_l = unit vector pointing at angle theta + 60 degrees
    eye_r = unit vector pointing at angle theta - 60 degrees
    move_l += max(0.0, dir . eye_l) * lamp_power / dist_sq
    move_r += max(0.0, dir . eye_r) * lamp_power / dist_sq

move_l = min(max_movement, move_l)
move_r = min(max_movement, move_r)

theta += (move_r - move_l) / wheel_base
forward_dist = (move_r + move_l) / 2
move robot forward_dist in the theta direction

int seconds = 0;
long nanoseconds = 40 * 1000 * 1000;
struct timespec interval = { seconds, nanoseconds };
nanosleep(&interval, NULL);

}
