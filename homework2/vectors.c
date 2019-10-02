#include "bmp.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//FUNCTIONS
void vector_create(vector_t *v) {
    v->capacity = 2;
    v->size = 0;
    v->data = malloc(sizeof(points_t *) * v->capacity);
}

void pg_create(vector_t *v) {
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

void pg_append(vector_t *v, int xvalue, int yvalue) {
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

void bresenham(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color) {
  points_t p = {0};
  vector_t p_vec = {0};
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


void pg_draw(bitmap_t *bmp, color_bgr_t color, pg_points_t *points){
  pg_points_t rect = {0};
  vector_t rect_vec = {0};

  vector_free(&rect_vec);
}

pg_points_t *give_rect(double width, double height){

  pg_create(&rect_vec);
  double xc = 0.0;
  double yc = 0.0;

  double p1x = xc + width/2.0;
  double p1y = yc + height/2.0;
  pg_append(&rect_vec,p1x, p1y);
  double p2x = xc + width/2.0;
  double p2y = yc - height/2.0;
  pg_append(&rect_vec,p2x, p2y);
  double p3x = xc - width/2.0;
  double p3y = yc - height/2.0;
  pg_append(&rect_vec,p3x, p3y);
  double p4x = xc - width/2.0;
  double p4y = yc + height/2.0;
  pg_append(&rect_vec,p4x, p4y);


  return rect;
}
