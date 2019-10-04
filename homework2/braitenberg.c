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
  if (argc != 3) {
      fprintf(stderr, "Error: Wrong number of arguments\n");
      return 1;
  }
  int timesteps = atoi(argv[1]);
  int fast = atoi(argv[1]);

  printf("%d\n", timesteps);
  printf("%d\n", fast);
}
