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

typedef struct pg_vector {
    points_t *data;
    int capacity;
    int size;
} pg_vector_t;

void vector_create(vector_t *v);

void vector_append(vector_t *v, int xvalue, int yvalue);

void vector_free(vector_t *v);

void bresenham(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color);

pg_points_t *give_rect(double width, double height, double xc, double yc);

pg_points_t cd2pixel(pg_points_t *points);

void pg_draw(bitmap_t *bmp, color_bgr_t color, pg_points_t *points);
