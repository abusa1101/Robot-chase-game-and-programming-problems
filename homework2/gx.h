#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bmp.h"

typedef struct points {
    double x;
    double y;
} points_t;

typedef struct vector_xy {
    pg_points_t *data;
    int capacity;
    int size;
} vector_xy_t;
