#pragma once

#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "image_server.h"
#include "bmp.h"
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
#define MAX_DEPTH 3
#define ROB_W 20
#define ROB_L (ROB_W * 4 / 3.0)

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

typedef struct state {
    int user_action;
    robot_t runner;
    robot_t chaser;
    int initial_runner_idx; // initially 17
    int delay_every; // must be >= 1
    double to_goal_magnitude;
    int to_goal_power; //[-3,3]
    double avoid_obs_magnitude;
    int avoid_obs_power; //[-3,3]
    int max_velocity; //[1,12]
    int current_parameter;
} state_t;

//Vector/Init/Low-level Operations
vector_xy_t *vector_create(void);
void vector_append(vector_xy_t *v, double xvalue, double yvalue);
void vector_free(vector_xy_t *v);
double max(double a, double b);
double min(double a, double b);

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
void wall(bitmap_t *bmp, color_bgr_t color_sq, double x, double y);
void robot(bitmap_t *bmp, color_bgr_t color, double x, double y, double theta);
void gx_draw_chaser(bitmap_t *bmp, state_t *state);
void give_runner_pos(state_t *state, int run_index);
void gx_draw_runner(bitmap_t *bmp, state_t *state, int run_index);
void gx_draw_game(bitmap_t *bmp, state_t *state);
vector_xy_t *robot2(robot_t *robot);

//Movement
void init_values(bitmap_t *bmp, state_t *state);
void serving_img(bitmap_t bmp);
void move(robot_t *robot);
void runner_walks(robot_t *runner);
void chaser_movement(state_t *state);

//Collision & Polygons
int pg_collision(vector_xy_t *pg1, vector_xy_t *pg2);
bool pg_intersection(vector_xy_t *pg1, vector_xy_t *pg2);
bool line_intersection(double x0, double y0, double x1, double y1,
                       double x2, double y2, double x3, double y3);
bool check4containment(vector_xy_t *pg, double x, double y);
bool robots_collision(robot_t *chaser, robot_t *runner);
bool tile_collision(robot_t *robot, double tile_x, double tile_y);
bool resolve_tile_collision(robot_t *robot);

//Thread/IO
void reset_terminal(void);
void *io_thread(void *user);

//Potential Field
void play_game(state_t *state);
