#include "bmp.h"
#include "chase_functions.h"
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

// Vector Operations
vector_xy_t *vector_create(void) {
    vector_xy_t *v = malloc(sizeof(vector_xy_t));
    v->capacity = 8;
    v->size = 0;
    v->data = malloc(sizeof(points_t) * v->capacity);
    return v;
}

void vector_append(vector_xy_t *v, double xvalue, double yvalue) {
    if (v->capacity == v->size) {
        v->data = realloc(v->data, sizeof(points_t) * (2 * v->capacity));
        v->capacity *= 2;
    }
    v->data[v->size].x = xvalue;
    v->data[v->size].y = yvalue;
    v->size++;
}

void vector_free(vector_xy_t *v) {
    free(v->data);
    free(v);
}

//GX Functions
vector_xy_t *gx_rect(double width, double height) {
    vector_xy_t *points = vector_create();

    double p1x = width;
    double p1y = 0;
    double p2x = width;
    double p2y = height;
    double p3x = 0;
    double p3y = height;
    double p4x = 0;
    double p4y = 0;

    vector_append(points, p4x, p4y);
    vector_append(points, p3x, p3y);
    vector_append(points, p2x, p2y);
    vector_append(points, p1x, p1y);

    return points;
}

void gx_rasterize_line(vector_xy_t *pathpoints, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (1) {
        vector_append(pathpoints, x0, y0);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

vector_xy_t *call_rasterize(vector_xy_t *points) {
    vector_xy_t *pathpoints = vector_create();
    for (int i = 0; i < points->size - 1; i++) {
        gx_rasterize_line(pathpoints, (int)points->data[i].x, (int)points->data[i].y,
                          (int)points->data[i + 1].x, (int)points->data[i + 1].y);
    }
    gx_rasterize_line(pathpoints, (int)points->data[points->size - 1].x,
                      (int)points->data[points->size - 1].y,
                      (int)points->data[0].x, (int)points->data[0].y);
    vector_free(points);
    return pathpoints;
}

void gx_rot(double theta, vector_xy_t *points) {
    for (int i = 0; i < points->size; i++) {
        double rot_x = (points->data[i].x * cos(theta) + points->data[i].y * sin(theta));
        double rot_y = (points->data[i].x * -sin(theta) + points->data[i].y * cos(theta));
        points->data[i].x = rot_x;
        points->data[i].y = rot_y;
    }
}

void gx_trans(double x, double y, vector_xy_t *points) {
    for (int i = 0; i < points->size; i++) {
        points->data[i].x += x;
        points->data[i].y += y;
    }
}

void gx_round(vector_xy_t *pathpoints) {
    double epsilon = 1e-6;
    double tempx = pathpoints->data[0].x;
    double tempy = pathpoints->data[0].y;
    for (int i = 0; i < pathpoints->size; i++) {
        if (tempx > pathpoints->data[i].x) {
            tempx = pathpoints->data[i].x;
        }
        if (tempy > pathpoints->data[i].y) {
            tempy = pathpoints->data[i].y;
        }
    }
    for (int i = 0; i < pathpoints->size; i++) {
        if (pathpoints->data[i].x == tempx) {
            pathpoints->data[i].x = ceil(pathpoints->data[i].x);
        } else {
            pathpoints->data[i].x = floor(pathpoints->data[i].x - epsilon);
        }
        if (pathpoints->data[i].y == tempy) {
            pathpoints->data[i].y = ceil(pathpoints->data[i].y);
        } else {
            pathpoints->data[i].y = floor(pathpoints->data[i].y - epsilon);
        }
    }
}

void gx_fill(bitmap_t *bmp, color_bgr_t color, vector_xy_t *pathpoints) {
    int x0[bmp->height];
    int x1[bmp->height];
    for (int i = 0; i < bmp->height; i++) {
        x0[i] = -1;
        x1[i] = -1;
    }
    for (int j = 0; j < pathpoints->size; j++) {
        int x = (int)pathpoints->data[j].x;
        int y = (int)pathpoints->data[j].y;
        if (y < 0 || y >= bmp->height) {
            continue;
        }
        if (x0[y] == -1) {
            x0[y] = x;
            x1[y] = x;
        } else {
            if (x0[y] > x) {
                x0[y] = x;
            }
            if (x0[y] < x) {
                x1[y] = x;
            }
        }
    }
    vector_free(pathpoints);

    for (int y = 0; y < bmp->height; y++) {
        if (x0[y] != -1) {
            int xmin = x0[y] > 0 ? x0[y] : 0;
            int xmax = x1[y] < (bmp->width - 1) ? x1[y] : (bmp->width - 1);
            for (int x = xmin; x <= xmax; x++) {
                bmp->data[y * bmp->width + x] = color;
            }
        }
    }
}

void gx_set_backgound(bitmap_t *bmp) {
    color_bgr_t white_backdrop = {255, 255, 255};
    for (int y = 0; y < bmp->height; y++) {
        for (int x = 0; x < bmp->width; x++) {
            bmp->data[y * bmp->width + x] = white_backdrop;
        }
    }
}

void wall (bitmap_t *bmp, color_bgr_t color_sq, double x, double y) {
    vector_xy_t *points = gx_rect(BLOCK_SIZE, BLOCK_SIZE);
    gx_rot(0, points);
    gx_trans(x, y, points);
    gx_round(points);
    vector_xy_t *pathpoints = call_rasterize(points);
    gx_fill(bmp, color_sq, pathpoints);
}

void gx_draw_game(bitmap_t *bmp) {
    gx_set_backgound(bmp);
    color_bgr_t color_sq = {0, 0, 0};

    for (int i = 0; i < MAP_H * MAP_W; i++) {
        if (MAP[i] == 'X') {
            wall (bitmap_t *bmp, color_bgr_t color_sq, (i % MAP_W), (int) i / MAP_W);
        }
    }
}

// void gx_draw_chaser(bitmap_t *bmp, double x, double y, double theta) {
//     color_bgr_t color_chaser = {0, 0, 255};
//     vector_xy_t *points = gx_robot(21, 28);
//     gx_rot(theta, points);
//     gx_trans(x, y, points); //starts at center of image
//     gx_round(points);
//     vector_xy_t *pathpoints = call_rasterize(points);
//     gx_fill(bmp, color_chaser, pathpoints);
// }
//
// void gx_draw_runner(bitmap_t *bmp, double x, double y, double theta) {
//     color_bgr_t color_runner = {0, 255, 0};
//     vector_xy_t *points = gx_robot(21, 28);
//     gx_rot(theta, points);
//     gx_trans(x, y, points); //starts at center of map loc
//     gx_round(points);
//     vector_xy_t *pathpoints = call_rasterize(points);
//     gx_fill(bmp, color, pathpoints);
// }
//
// void give_runner_pos(void) {
//     int map_idx_x = initial_runner_idx % MAP_W;
//     int map_idx_y = initial_runner_idx / MAP_W;
//     state->runner.x = (map_idx_x + 0.5) * BLOCK_SIZE;
//     state->runner.y = (map_idx_y + 0.5) * BLOCK_SIZE;
// }
// void robot_move(void) {
//     apply action
//     theta += angular velocity
//     angular velocity *= 0.8
//     move robot by velocity in the theta direction
//     resolve collisions
//     if robot had any collision with a wall {
//         velocity *= 0.25
//     }
// }
