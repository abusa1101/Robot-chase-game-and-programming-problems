#include "bmp.h"
#include "all_functions.h"
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

typedef struct runner {
    points_t pos;
    double theta;
    double fwd_vel;
    double ang_vel;
} runner_t;

typedef struct chaser {
    points_t pos;
    double theta;
    double fwd_vel;
    double ang_vel;
} chaser_t;

typedef struct robots {
    chaser_t chaser;
    runner_t runner;
} chaser_t;

void gx_set(bitmap_t *bmp) {
    color_bgr_t white_backdrop = {255, 255, 255};
    for (int y = 0; y < bmp->height; y++) {
        for (int x = 0; x < bmp->width; x++) {
            bmp->data[y * bmp->width + x] = white_backdrop;
        }
    }
}

void gx_draw_game(bitmap_t *bmp, double x, double y) {
    gx_set(bmp);
    color_bgr_t color_sq = {0, 0, 0};
    vector_xy_t *points = gx_rect(BLOCK_SIZE, BLOCK_SIZE);
    gx_rot(0, points);
    gx_trans(x, y, points);
    gx_round(points);
    vector_xy_t *pathpoints = call_rasterize(points);
    gx_fill(bmp, color_sq, pathpoints);
}

void gx_draw_chaser(bitmap_t *bmp, double x, double y, double theta) {
    color_bgr_t color_chaser = {0, 0, 255};
    vector_xy_t *points = gx_robot(21, 28);
    gx_rot(theta, points);
    gx_trans(x, y, points); //starts at center of image
    gx_round(points);
    vector_xy_t *pathpoints = call_rasterize(points);
    gx_fill(bmp, color_chaser, pathpoints);
}

void gx_draw_runner(bitmap_t *bmp, double x, double y, double theta) {
    color_bgr_t color_runner = {0, 255, 0};
    vector_xy_t *points = gx_robot(21, 28);
    gx_rot(theta, points);
    gx_trans(x, y, points); //starts at center of map loc
    gx_round(points);
    vector_xy_t *pathpoints = call_rasterize(points);
    gx_fill(bmp, color, pathpoints);
}

void give_runner_pos(void) {
    int map_idx_x = initial_runner_idx % MAP_W;
    int map_idx_y = initial_runner_idx / MAP_W;
    state->runner.x = (map_idx_x + 0.5) * BLOCK_SIZE;
    state->runner.y = (map_idx_y + 0.5) * BLOCK_SIZE;
}
void robot_move(void) {
    apply action
    theta += angular velocity
    angular velocity *= 0.8
    move robot by velocity in the theta direction
    resolve collisions
    if robot had any collision with a wall {
        velocity *= 0.25
    }
}
