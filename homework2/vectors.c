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

void pg_create(pg_vector_t *v) {
    v->capacity = 2;
    v->size = 0;
    v->data = malloc(sizeof(pg_points_t *) * v->capacity);
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

void pg_append(pg_vector_t *v, int xvalue, int yvalue) {
    if (v->capacity == v->size) {
        v->data = realloc(v->data, sizeof(pg_points_t *) * (2 * v->capacity));
        v->capacity *= 2;
    }
    v->data[v->size].x = xvalue;
    v->data[v->size].y = yvalue;
    v->size++;
}

void vector_free(vector_t *v) {
    free(v->data);
}

void pg_free(pg_vector_t *v) {
    free(v->data);
}

void bresenham(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color) {
  //points_t p = {0};
  //vector_t p_vec = {0};
  //vector_create(&p_vec);
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

pg_vector_t *give_rect(double width, double height, double xc, double yc){
  pg_vector_t rect_vec;
  pg_create(&rect_vec);
  double p1x = xc + width / 2.0;
  double p1y = yc + height / 2.0;
  if (p1x >= 0 && p1y >= 0) {
    pg_append(&rect_vec,p1x, p1y);
  }
  double p2x = xc + width / 2.0;
  double p2y = yc - height / 2.0;
  if (p2x >= 0 && p2y >= 0) {
    pg_append(&rect_vec,p2x, p2y);
  }
  double p3x = xc - width / 2.0;
  double p3y = yc - height / 2.0;
  if (p3x >= 0 && p3y >= 0) {
    pg_append(&rect_vec,p3x, p3y);
  }
  double p4x = xc - width / 2.0;
  double p4y = yc + height / 2.0;
  if (p4x >= 0 && p4y >= 0) {
    pg_append(&rect_vec,p4x, p4y);
  }
  return &rect_vec;
} //MUST FREE STILL!!!!!!

//maybe dont need this function? could just replace initial values with its rounded self
pg_vector_t *cd2pixel(pg_vector_t *points) {
  pg_vector_t pixels;
  pg_create(&pixels);
  double epsilon = 1e-6;
  double tempx = points->data[0].x; //find a better way to do this perhaps??
  double tempy = points->data[0].y;
  for(int i = 0; i <points->size; i++) { //find min value and store in tempx,tempy
    if (tempx > points->data[i].x) {
      tempx = points->data[i].x;
    }
    if (tempy > points->data[i].y) {
      tempy = points->data[i].y;
    }
  }
  for(int i = 0; i <points->size; i++) { //check for min value and round off
    if (points->data[i].x == tempx) {
      pixels.data[i].x = ceil(points->data[i].x);
    } else {
      pixels.data[i].x = floor(points->data[i].x - epsilon);
    }
    if (points->data[i].y == tempy) {
      pixels.data[i].y = ceil(points->data[i].y);
    } else {
      pixels.data[i].y = floor(points->data[i].y - epsilon);
    }
  }
  pg_free(points);
  return &pixels;
} //MUST FREE THIS AS WELL!!!

void pg_draw(bitmap_t *bmp, color_bgr_t color, pg_points_t *points) {
  pg_vector_t *pixelpts = cd2pixel(points);
  for(int i = 0; i < pixelpts->size; i++){
    bresenham(pixelpts->data[i].x, pixelpts->data[i].y, pixelpts->data[i+1].x, pixelpts->data[i+1].y, bmp, color);
  }
  bresenham(pixelpts->data[pixelpts.size - 1].x, pixelpts->data[pixelpts.size - 1].y, pixelpts->data[0].x, pixelpts->data[0].y, bmp, color);
  pg_free(pixelpts);
} //MUST FREE STILL!!!!!!!!!!!!!!!!
