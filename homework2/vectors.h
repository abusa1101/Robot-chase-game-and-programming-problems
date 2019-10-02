#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct points {
    int x;
    int y;
} points_t;

//vector structure
typedef struct vector {
    points_t *data;
    int capacity;
    int size;
} vector_t;

typedef struct pg_points { //clockwise winding points
    double x;
    double y;
} pg_points_t;

void vector_create(vector_t *v, struct points);
void vector_append(vector_t *v, struct points, int xvalue, int yvalue);
void vector_free(vector_t *v);
void bresenham(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color);
