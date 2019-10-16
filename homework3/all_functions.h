#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "bmp.h"

typedef struct points {
    double x;
    double y;
} points_t;

typedef struct vector_xy {
    points_t *data;
    int capacity;
    int size;
} vector_xy_t;

typedef struct unitvec {
    double x;
    double y;
} unitvec_t;

typedef struct lamps {
    unitvec_t location;
} lamp_t;

typedef struct game {
    unitvec_t rpos;
    double rtheta;
    unitvec_t lpos[3];
    int n_lamp;
} game_t;

// Vector Operations
vector_xy_t *vector_create(void);
void vector_append(vector_xy_t *v, double xvalue, double yvalue);
void vector_free(vector_xy_t *v);

//GX
void gx_rasterize_line(vector_xy_t *pathpoints, int x0, int y0, int x1, int y1);
vector_xy_t *call_rasterize(vector_xy_t *points);
void gx_draw_line(bitmap_t *bmp, color_bgr_t color, int x0, int y0, int x1, int y1);
void gx_draw(bitmap_t *bmp, color_bgr_t color, vector_xy_t *pathpoints);
void gx_fill(bitmap_t *bmp, color_bgr_t color, vector_xy_t *pathpoints);
vector_xy_t *gx_rect(double width, double height);
vector_xy_t *gx_robot(double w, double h);
void gx_rot(double theta, vector_xy_t *points);
void gx_trans(double x, double y, vector_xy_t *points);
void gx_round(vector_xy_t *pathpoints);

//Objects
void lamp(bitmap_t *bmp, color_bgr_t color, double x, double y);
void robot(bitmap_t *bmp, color_bgr_t color, double x, double y, double theta);
void backdrop(bitmap_t *bmp, color_bgr_t color);

//Movement
void gx_update(bitmap_t *bmp, game_t *game, color_bgr_t color_back,
               color_bgr_t color_lamp, color_bgr_t color_robot);
void gx_clear(bitmap_t *bmp);
void init_values(game_t *game);
double dotP(double x0, double y0, double x1, double y1);
void activateMove(game_t *game);

//Collision
bool robot_collision(game_t *game, vector_xy_t *lamp);
void resolve_collision(game_t *game, double lamp_x, double lamp_y);
bool line_intersection(double x0, double y0, double x1, double y1,
                       double x2, double y2, double x3, double y3);
bool pg_intersection(vector_xy_t *pg1, vector_xy_t *pg2);
bool check4containment(vector_xy_t *pg, double x, double y);
int pg_collision(vector_xy_t *pg1, vector_xy_t *pg2);
