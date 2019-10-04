#include "bmp.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//FUNCTIONS
void vector_create(vector_t *v) {
    v->capacity = 2;
    v->size = 0;
    v->data = malloc(sizeof(points_t *) * v->capacity);
}

void vector_append(vector_t *v, int xvalue, int yvalue) {
    if (v->capacity == v->size) {
        v->data = realloc(v->data, sizeof(points_t *) * (2 * v->capacity));
        v->capacity *= 2;
    }
    v->data[v->size].x = xvalue;
    v->data[v->size].y = yvalue;
    v->size++;
}

void vector_free(vector_t *v) {
    free(v->data);
}

void pg_create(pg_vector_t *v) {
    v->capacity = 2;
    v->size = 0;
    v->data = malloc(sizeof(pg_points_t *) * v->capacity);
}

void pg_append(pg_vector_t *v, double xvalue, double yvalue) {
  printf("%f ", xvalue);
  printf("%f\n", yvalue);
    if (v->capacity == v->size) {
        v->data = realloc(v->data, sizeof(pg_points_t *) * (2 * v->capacity));
        v->capacity *= 2;
    }
    v->data[v->size].x = (double) xvalue;
    v->data[v->size].y = (double) yvalue;
    printf("%d ", v->data[v->size].x);
    printf("%d\n", v->data[v->size].y);
    v->size++;

}

void pg_free(pg_vector_t *v) {
    free(v->data);
}

void bresenham(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color) {
  //points_t p = {0};
  //vector_t p_vec = {0};
  //vector_create(&p_vec);
  if (x1 == 0 && y1 == 0){
    return;
  }
  double dx =  abs(x1 - x0);
  double sx = x0 < x1 ? 1 : -1;
  double dy = -abs(y1 - y0);
  double sy = y0 < y1 ? 1 : -1;
  double err = dx + dy;
  double e2;

  while (1) {
    if (x0 == x1 && y0 == y1) {
      break;
    }
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
    bmp->data[y0 * bmp->width + x0] = color;
    //vector_append(&p_vec,x0, y0);
  }
  //vector_free(&p_vec);
}

void give_rect(pg_vector_t *rect_vec, double width, double height, double xc, double yc) {
  double p1x = xc + width / 2.0;
  double p1y = yc + height / 2.0;
  // printf("%f ", p1x);
  // printf("%f\n", p1y);
  if (p1x < 0) {
    p1x = 0;
  }
  if (p1y < 0) {
    p1y = 0;
  }
  double p2x = xc + width / 2.0;
  double p2y = yc - height / 2.0;
  // printf("%f ", p2x);
  // printf("%f\n", p2y);
  if (p2x < 0) {
    p2x = 0;
  }
  if (p2y < 0) {
    p2y = 0;
  }
  double p3x = xc - width / 2.0;
  double p3y = yc - height / 2.0;
  // printf("%f ", p3x);
  // printf("%f\n", p3y);
  if (p3x < 0) {
    p3x = 0;
  }
  if (p3y < 0) {
    p3y = 0;
  }
  double p4x = xc - width / 2.0;
  double p4y = yc + height / 2.0;
  // printf("%f ", p4x);
  // printf("%f\n", p4y);
  if (p4x < 0) {
    p4x = 0;
  }
  if (p4y < 0) {
    p4y = 0;
  }
  pg_append(rect_vec,p4x, p4y);
  pg_append(rect_vec,p3x, p3y);
  pg_append(rect_vec,p2x, p2y);
  pg_append(rect_vec,p1x, p1y);
  // for(int i = 0; i <rect_vec->size; i++) {
  //   printf("%f ", rect_vec->data[i].x);
  //   printf("%f\n", rect_vec->data[i].y);
  // }
}

void cd2pixel(pg_vector_t *rect_vec) {
  double epsilon = 1e-6;
  double tempx = rect_vec->data[0].x; //find a better way to do this perhaps??
  double tempy = rect_vec->data[0].y;
  for(int i = 0; i <rect_vec->size; i++) { //find min value and store in tempx,tempy
    if (tempx > rect_vec->data[i].x) {
      tempx = rect_vec->data[i].x;
    }
    if (tempy > rect_vec->data[i].y) {
      tempy = rect_vec->data[i].y;
    }
  }
  for(int i = 0; i <rect_vec->size; i++) { //check for min value and round off
    if (rect_vec->data[i].x == tempx) {
      rect_vec->data[i].x = ceil(rect_vec->data[i].x);
    } else {
      rect_vec->data[i].x = floor(rect_vec->data[i].x - epsilon);
    }
    if (rect_vec->data[i].y == tempy) {
      rect_vec->data[i].y = ceil(rect_vec->data[i].y);
    } else {
      rect_vec->data[i].y = floor(rect_vec->data[i].y - epsilon);
    }
  }
}

void translate(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double xglobal, double yglobal) {
  //printf("\n");
  for(int i = 0; i < rect_vec->size ; i++){
    //give_rect(&rect_vec, 100.0, 100.0, xc +, yc);
    double transx = rect_vec->data[i].x + xglobal;
    double transy = rect_vec->data[i].y + yglobal;
    pg_append(transformed_vec,transx, transy);
    // printf("%d ", transformed_vec->data[i].x);
    // printf("%d\n", transformed_vec->data[i].y);
  }
}

// double xpivot = 0.0; //rotation pivot point
// double ypivot = 0.0;
// for(int i = 0; i < rect_vec->size - 1; i++){
// printf("%d ", transformed_vec->data[i].x);
// printf("%d\n", transformed_vec->data[i].y);
// //rotation
// double rotx = xpivot + ((rect_vec->data[i].x - xpivot) * cos(radrot)
//                      - rect_vec->data[i].x - ypivot * sin(radrot));
// double roty = ypivot + ((rect_vec->data[i].y - xpivot) * sin(radrot)
//                      + rect_vec->data[i].y - ypivot * cos(radrot));
// pg_append(transformed_vec,rotx, roty);
// printf("%d ", transformed_vec->data[i].x);
// printf("%d\n", transformed_vec->data[i].y);
// }


void pg_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec) {
  //printf("%d ", rect_vec->size);
  for(int i = 0; i < rect_vec->size - 1; i++){
    bresenham(rect_vec->data[i].x, rect_vec->data[i].y, rect_vec->data[i+1].x, rect_vec->data[i+1].y, bmp, color);
  }
  bresenham(rect_vec->data[rect_vec->size - 1].x, rect_vec->data[rect_vec->size - 1].y, rect_vec->data[0].x, rect_vec->data[0].y, bmp, color);
}
