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

typedef struct robot {
    bool is_runner;
    double x;
    double y;
    double theta;
    double fwd_vel;
    double ang_vel;
} robot_t;

typedef struct search_node {
    int depth;
    robot_t runner;
    robot_t chaser;
} search_node_t;

typedef struct state {
    bool is_runner_caught;
    robot_t runner;
    robot_t chaser;
} state_t;

// Vector Operations
vector_xy_t *vector_create(void);
void vector_append(vector_xy_t *v, double xvalue, double yvalue);
void vector_free(vector_xy_t *v);

//GX Functions
vector_xy_t *gx_rect(double width, double height);
vector_xy_t *gx_robot(double w, double h);
void gx_rasterize_line(vector_xy_t *pathpoints, int x0, int y0, int x1, int y1);
vector_xy_t *call_rasterize(vector_xy_t *points);
void gx_rot(double theta, vector_xy_t *points);
void gx_trans(double x, double y, vector_xy_t *points);
void gx_round(vector_xy_t *pathpoints);
void gx_fill(bitmap_t *bmp, color_bgr_t color, vector_xy_t *pathpoints);
void gx_set_backgound(bitmap_t *bmp);
void wall (bitmap_t *bmp, color_bgr_t color_sq, double x, double y);
void robot(bitmap_t *bmp, color_bgr_t color, double x, double y, double theta);
void gx_draw_game(bitmap_t *bmp, state_t *state, int run_index);
void gx_draw_chaser(bitmap_t *bmp,  state_t *state);
void give_runner_pos(state_t *state, int run_index);
void gx_draw_runner(bitmap_t *bmp, state_t *state, int run_index);

//Movement
void init_values(state_t *state);
