//#ifndef MOVEMENT_H
//#define MOVEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bmp.h"
#define M_PI 3.14159265358979323846

typedef struct pg_points {
    double x;
    double y;
} pg_points_t;

typedef struct pg_vector {
    pg_points_t *data;
    int capacity;
    int size;
} pg_vector_t;
// typedef struct pos_data {
//     double x;
//     double y;
// } pos_data_t;
//
// typedef struct rob_position {
//     pos_data_t *data;
//     int capacity;
//     int size;
// } rob_position_t;
//
// typedef struct unitvec {
//     double x;
//     double y;
// } unitvec_t;
//
// typedef struct lamp {
//     rob_position_t location;
//     color_bgr_t color;
//     double size;
//     double rot;
//     double power;
// } lamp_t;
//
// typedef struct compare {
//     double l1;
//     double l2;
//     double l3;
// } compare_t;
//
// typedef struct robot {
//   rob_position_t location;
//   color_bgr_t color;
//   unitvec_t dir;
//   unitvec_t eye_l;
//   unitvec_t eye_r;
//   double dist_sq;
//   double move_l;
//   double move_r;
//   double theta;
//   double fwd_dist;
//   double max_movement;
//   double wheel_base;
//   double width;
//   double length;
// } robot_t;
//
// typedef struct game {
//   color_bgr_t color;
//   robot_t robot;
//   lamp_t lamp;
//   double n_lamp;
// } game_t;

////////////////////////////////////////////FUNCTIONS/////////////////////////////////////////////////////////////
//Movement
//void setConstants(robot_t robot, lamp_t lamp1, lamp_t lamp2, lamp_t lamp3);
// // double dotP(game_t game);
// // void rotateP(double dir, double angle, double x_pivot, double y_pivot);
// // void activateMove(game_t game);
//
//Graphics
 //void initGraphics(game_t game, pg_vector_t *rect_vec);
// // void resetGraphics (game_t game, pg_points_t *rect_vec);
// // void updateGraphics(game_t game, pg_points_t *rect_vec);
//
// //Collision
// // int line_intersection(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);
// // int pg_intersection(polygon_t *pg1, polygon_t *pg2);
// // int pg_collision(polygon_t *pg1, polygon_t *pg2);
// // int check4containment(polygon_t *pg, double x, double y);
//


void pg_create(pg_vector_t *v);

void pg_create2(pg_vector_t *v);

void pg_append(pg_vector_t *v, double xvalue, double yvalue);

void pg_free(pg_vector_t *v);

void bresenham(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color);

void bresenham2(pg_vector_t *fillbuff, int x0, int y0, int x1, int y1);

void callb2(pg_vector_t *fillbuff, pg_vector_t *transformed_vec);

void give_rect(pg_vector_t *rect_vec, double width, double height, double xc, double yc);

void cd2pixel(pg_vector_t *rect_vec);

void translate(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double xglobal, double yglobal);

void pg_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec, int n);

void pg_fill(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec);

void give_tri(pg_vector_t *tri_vec, double w, double h, double xc, double yc);

void tri_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *tri_vec);

void tri_fill(bitmap_t *bmp, color_bgr_t color, pg_vector_t *tri_vec, pg_vector_t *fillbuff);

void rotate(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double angle);

void rotate2(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double angle, double x_pivot, double y_pivot);


//#endif
