
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846

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
    pg_points_t *data;
    int capacity;
    int size;
} pg_vector_t;

void vector_create(vector_t *v);

void vector_append(vector_t *v, int xvalue, int yvalue);

void vector_free(vector_t *v);

void pg_create(pg_vector_t *v);

void pg_append(pg_vector_t *v, double xvalue, double yvalue);

void pg_free(pg_vector_t *v);

void bresenham(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color);

void give_rect(pg_vector_t *rect_vec, double width, double height, double xc, double yc);

void cd2pixel(pg_vector_t *rect_vec);

void translate(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double xglobal, double yglobal);

void pg_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec, int n);

void pg_fill(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec);

void give_tri(pg_vector_t *tri_vec, double w, double h, double xc, double yc);

void tri_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *tri_vec);

void tri_fill(bitmap_t *bmp, color_bgr_t color, pg_vector_t *tri_vec);

void rotate(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double degrot);
