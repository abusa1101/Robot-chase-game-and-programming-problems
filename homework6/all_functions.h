#pragma once

#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <lcm/lcm.h>
#include "image_server.h"
#include "bmp.h"
#include "lcmtypes/settings_t.h"
#include "lcmtypes/reset_t.h"
#include "lcmtypes/action_t.h"
#include "lcmtypes/world_t.h"
#include "lcmtypes/agent_t.h"
#include "lcm_handle_async.h"

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
#define SLEEP_40 40

typedef struct points {
    double x;
    double y;
} points_t;

typedef struct vector_xy {
    points_t *data;
    int capacity;
    int size;
} vector_xy_t;

typedef struct state {
    int user_action;
    agent_t runner;
    agent_t chaser;
    int initial_runner_idx; //initially 17
    int delay_every; //must be >= 1
    double to_goal_magnitude;
    int to_goal_power; //[-3,3]
    double avoid_obs_magnitude;
    int avoid_obs_power; //[-3,3]
    int max_vel; //[1,12]

    int current_parameter;
    int timestep;
    lcm_t *lcm;
} state_t;

//Vector/Init/Low-level Operations
vector_xy_t *vector_create(void);
void vector_append(vector_xy_t *v, double xvalue, double yvalue);
void vector_free(vector_xy_t *v);
double max(double a, double b);
double min(double a, double b);
int constrain(int val, int LL, int UL);
void bmp_init(bitmap_t *bmp);
int change_robot_idx(int idx, bool is_next);

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
vector_xy_t *robot2(agent_t *robot);

//Collision & Polygons
int pg_collision(vector_xy_t *pg1, vector_xy_t *pg2);
bool pg_intersection(vector_xy_t *pg1, vector_xy_t *pg2);
bool line_intersection(double x0, double y0, double x1, double y1,
                       double x2, double y2, double x3, double y3);
bool check4containment(vector_xy_t *pg, double x, double y);
bool robots_collision(agent_t *chaser, agent_t *runner);
bool tile_collision(agent_t *robot, double tile_x, double tile_y);
bool resolve_tile_collision(agent_t *robot);

//Movement
void init_values(state_t *state);
void serving_img(bitmap_t bmp, state_t *state);
void move(state_t *state, agent_t *robot);
void runner_walks(state_t *state);
void chaser_moves(state_t *state);

//Thread/IO
void reset_terminal(void);
void *io_thread(void *user);

//Potential Field
void potential_field_control(state_t *state);
void update_parameters(state_t *state, bool action_is_up);
void reset_simulation(state_t *state);
