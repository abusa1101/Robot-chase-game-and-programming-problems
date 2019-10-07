

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bmp.h"
#define M_PI 3.14159265358979323846

typedef struct polygon {
  int npoints;
  double x;
  double y;
} polygon_t;

typedef struct vector {
    polygon_t *data;
    int capacity;
    int size;
} vector_t;

typedef struct pos_data {
    double x;
    double y;
} pos_data_t;

typedef struct rob_position {
    pos_data_t *data;
    int capacity;
    int size;
} rob_position_t;

typedef struct unitvec {
    double x;
    double y;
} unitvec_t;

typedef struct lamp {
    pos_data_t location;
    color_bgr_t color;
    double size;
    double power;
} lamp_t;

typedef struct compare {
    double l1;
    double l2;
    double l3;
} compare_t;

typedef struct robot {
  rob_position_t location;
  color_bgr_t color;
  unitvec_t dir;
  unitvec_t eye_l;
  unitvec_t eye_r;
  double dist_sq;
  double move_l;
  double move_r;
  double theta;
  double fwd_dist;
  double max_movement;
  double wheel_base;
} robot_t;

typedef struct game {
  color_bgr_t color;
  robot_t robot;
  lamp_t lamp;
  double n_lamp;
} game_t;

double dotP(double dir[], double eye[]) {
  double product = 0;
  for (int i = 0; i < 2; i++) { //2 because 2D ai + bj
    product = product + dir[i] * eye[i];
  }
  return product;
}

void rotateP(double dir, double angle, double x_pivot, double y_pivot) {
    double rotx = 0;
    double roty = 0;
    // int i = 0;
    // while (i < 2) {
        double x_shifted = dir[i] - x_pivot;
        double y_shifted = dir[i] - y_pivot;
        rotx = x_pivot + (x_shifted * COS(angle) - y_shifted * SIN(angle));
        roty = y_pivot + (x_shifted * SIN(angle) + y_shifted * COS(angle));
    //    pg_append(transformed_vec, rotx, roty);
    //     i++;
    // }
}

void activateLamp(lamp_t lamp, robot_t robot);

int line_intersection(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3) {
  double cp0 = (x3 - x2) * (y0 - y2) - (x0 - x2) * (y3 - y2); //cp for p0
  double cp1 = (x3 - x2) * (y1 - y2) - (x1 - x2) * (y3 - y2); //cp for p1
  double cp2 = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0); //cp for p2
  double cp3 = (x1 - x0) * (y3 - y0) - (x3 - x0) - (y1 - y0); //cp for p3

  double check_cp23 = cp2 * cp3;
  double check_cp01 = cp0 * cp1;

  if(check_cp01 <= 0 && check_cp23 <= 0) {
    if ((check_cp01 != 0 || check_cp23 != 0)) {
      return 1;
    }
  } else {
    return 0;
  }
}

int pg_intersection(polygon_t *pg1, polygon_t *pg2) {
  for (int i = 0; i < pg1->npoints; i++) {
    int i2 = (i + 1) % pg->npoints; //check hw2 document for this indexing reference
    double x0 = pg1->x[i];
    double y0 = pg1->y[i];
    double x1 = pg1->x[i2];
    double y1 = pg1->y[i2];
    for (int j = 0; j < pg2->npoints; j++) {
      int j2 = (j + 1) % pg2-> npoints;
      double x2 = pg2->x[j];
      double y2 = pg2->x[j];
      double x3 = pg2->x[j2];
      double y3 = pg2->x[j2];
      if (line_intersection(x0, y0, x1, y1, x2, y2, x3, y3) == 1) {
        return 1;
      } else {
        return 0;
      }
    }
  }
}

int pg_collision(polygon_t *pg1, polygon_t *pg2) {
  if (pg_intersection(pg1,pg2) == 1) {
    return 1;
  } else if (check4containment(pg1, pg2->x[0], pg2->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
    return 1;
  } else if (check4containment(pg2, pg1->x[0], pg1->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
    return 1;
  } else {
    return 0;
  }
}

int check4containment(polygon_t *pg, double x, double y) {
  int counter1 = 0;
  int counter2 = 0;

  for (int i = 0; i < pg-> npoints; i++) {
    int i2 = (i + 1) % pg->npoints;
    double cp = (pg->x[i2] - pg->x[i] * (y - pg->y[i])) - (x - pg->x[i] * (pg->y[i2] - pg->y[i]));
    if (cp > 0) {
      counter1++;
    } else if (cp < 0) {
      counter2++;
    }
  }

  if (counter1 == 0) {
    return 1;
  } else if (counter2 == 0) {
    return 1;
  } else {
    return 0;
  }
}


//////////////////////// RASTERIZE FUNCTIONS ///////////////////////////////////////////////
void pg_create(vector_t *v);

void pg_append(vector_t *v, double xvalue, double yvalue);

void pg_free(vector_t *v);

// void bresenham(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color);
//
// void bresenham2(pg_vector_t *fillbuff, int x0, int y0, int x1, int y1);
//
// void callb2(pg_vector_t *fillbuff, pg_vector_t *transformed_vec);
//
// void give_rect(pg_vector_t *rect_vec, double width, double height, double xc, double yc);
//
// void cd2pixel(pg_vector_t *rect_vec);
//
// void translate(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double xglobal, double yglobal);
//
// void pg_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec, int n);
//
// void pg_fill(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec);
//
// void give_tri(pg_vector_t *tri_vec, double w, double h, double xc, double yc);
//
// void tri_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *tri_vec);
//
// void tri_fill(bitmap_t *bmp, color_bgr_t color, pg_vector_t *tri_vec, pg_vector_t *fillbuff);
//
// void rotate2(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double angle, double x_pivot, double y_pivot);
//
// // void CallFunctions(bitmap_t bmp, color_bgr_t color, int input,
// //                    pg_vector_t rect_vec, pg_vector_t transformed_vec, pg_vector_t fillbuff);
// //
