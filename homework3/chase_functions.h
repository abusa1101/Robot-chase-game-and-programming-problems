#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define WIDTH 640
#define HEIGHT 480

#define BLOCK_SIZE 40
#define MAP "XXXXXXXXXXXXXXXX" \
            "X              X" \
            "X  XXXX   XXX  X" \
            "X   XX      X  X" \
            "X       XXX    X" \
            "XXXXXX         X" \
            "X         XXXXXX" \
            "X    XXX       X" \
            "X  XX     XX   X" \
            "X   X    XX    X" \
            "X      XXX     X" \
            "XXXXXXXXXXXXXXXX"
#define MAP_W (WIDTH / BLOCK_SIZE)
#define MAP_H (HEIGHT / BLOCK_SIZE)

typedef struct points {
    double x;
    double y;
} points_t;

typedef struct vector_xy {
    points_t *data;
    int capacity;
    int size;
} vector_xy_t;

// typedef struct runner {
//     points_t pos;
//     double theta;
//     double fwd_vel;
//     double ang_vel;
// } runner_t;
//
// typedef struct chaser {
//     points_t pos;
//     double theta;
//     double fwd_vel;
//     double ang_vel;
// } chaser_t;
//
// typedef struct robots {
//     chaser_t chaser;
//     runner_t runner;
// } chaser_t;

// Vector Operations
vector_xy_t *vector_create(void)
void vector_append(vector_xy_t *v, double xvalue, double yvalue);
void vector_free(vector_xy_t *v);

//GX Functions
vector_xy_t *gx_rect(double width, double height);
void gx_rasterize_line(vector_xy_t *pathpoints, int x0, int y0, int x1, int y1);
vector_xy_t *call_rasterize(vector_xy_t *points);
void gx_rot(double theta, vector_xy_t *points);
void gx_trans(double x, double y, vector_xy_t *points);
void gx_round(vector_xy_t *pathpoints);
void gx_fill(bitmap_t *bmp, color_bgr_t color, vector_xy_t *pathpoints);
void gx_set_backgound(bitmap_t *bmp);
void wall (bitmap_t *bmp, color_bgr_t color_sq, double x, double y);
void gx_draw_game(bitmap_t *bmp);
