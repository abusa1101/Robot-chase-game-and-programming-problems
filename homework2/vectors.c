#include "bmp.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define SIN(x) sin((x)*M_PI / 180)
#define COS(x) cos((x)*M_PI / 180)

void pg_create(pg_vector_t *v) {
    v->capacity = 10;
    v->size = 0;
    v->data = malloc(sizeof(pg_points_t *) * v->capacity);
}

void pg_create2(pg_vector_t *v) {
    v->capacity = 200;
    v->size = 0;
    v->data = malloc(sizeof(pg_points_t *) * v->capacity);
}

void pg_append(pg_vector_t *v, double xvalue, double yvalue) {
    if (v->capacity == v->size) {
        v->data = realloc(v->data, sizeof(pg_points_t *) * (2 * v->capacity));
        v->capacity *= 2;
    }
    v->data[v->size].x = xvalue;
    v->data[v->size].y = yvalue;
    v->size++;
}

void pg_free(pg_vector_t *v) {
    free(v->data);
}

void bresenham(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color) {
  int dx =  abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;
  while (1) {
      bmp->data[y0 * bmp->width + x0] = color;
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

void bresenham2(pg_vector_t *fillbuff, int x0, int y0, int x1, int y1) {
  int dx =  abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;
  while (1) {
    pg_append(fillbuff, x0, y0);
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

void callb2(pg_vector_t *fillbuff, pg_vector_t *transformed_vec) {
  for (int i = 0; i < transformed_vec->size - 1; i++) {
      bresenham2(fillbuff, (int)transformed_vec->data[i].x, (int)transformed_vec->data[i].y,
                (int)transformed_vec->data[i + 1].x, (int)transformed_vec->data[i + 1].y);
  }
  bresenham2(fillbuff, (int)transformed_vec->data[transformed_vec->size - 1].x, (int)transformed_vec->data[transformed_vec->size - 1].y,
            (int)transformed_vec->data[0].x, (int)transformed_vec->data[0].y);

  //int n = fillbuff->size;
   for (int i = 0; i <fillbuff->size; i++) {
    printf("%f ", fillbuff->data[i].x);
    printf("%f\n", fillbuff->data[i].y);
    //printf("%d ", i);
  }
}

void give_rect(pg_vector_t *rect_vec, double width, double height, double xc, double yc) {
    double p1x = xc + width / 2.0;
    double p1y = yc + height / 2.0;
    if (p1x < 0) {
      p1x = 0;
    }
    if (p1y < 0) {
      p1y = 0;
    }
    double p2x = xc + width / 2.0;
    double p2y = yc - height / 2.0;
    if (p2x < 0) {
      p2x = 0;
    }
    if (p2y < 0) {
      p2y = 0;
    }
    double p3x = xc - width / 2.0;
    double p3y = yc - height / 2.0;
    if (p3x < 0) {
      p3x = 0;
    }
    if (p3y < 0) {
      p3y = 0;
    }
    double p4x = xc - width / 2.0;
    double p4y = yc + height / 2.0;
    if (p4x < 0) {
      p4x = 0;
    }
    if (p4y < 0) {
      p4y = 0;
    }
    pg_append(rect_vec, p4x, p4y);
    pg_append(rect_vec, p3x, p3y);
    pg_append(rect_vec, p2x, p2y);
    pg_append(rect_vec, p1x, p1y);
}

void cd2pixel(pg_vector_t *rect_vec) {
    double epsilon = 1e-6;
    double tempx = rect_vec->data[0].x;
    double tempy = rect_vec->data[0].y;
    for (int i = 0; i < rect_vec->size; i++) {
        if (tempx > rect_vec->data[i].x) {
            tempx = rect_vec->data[i].x;
        }
        if (tempy > rect_vec->data[i].y) {
            tempy = rect_vec->data[i].y;
        }
    }
    for (int i = 0; i < rect_vec->size; i++) { //check for min value and round off
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

void translate(pg_vector_t *rect_vec, pg_vector_t *transformed_vec,
               double xglobal, double yglobal) {
    for (int i = 0; i < rect_vec->size; i++) {
        double transx = rect_vec->data[i].x + xglobal;
        double transy = rect_vec->data[i].y + yglobal;
        pg_append(transformed_vec, transx, transy);
    }
}

void pg_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec, int n) {
    if (n == 3) {
        bresenham(rect_vec->data[2].x, rect_vec->data[2].y, rect_vec->data[3].x,
                  rect_vec->data[3].y, bmp, color);
        bresenham(rect_vec->data[rect_vec->size - 1].x, rect_vec->data[rect_vec->size - 1].y,
                  rect_vec->data[0].x, rect_vec->data[0].y, bmp, color);
        return;
    }
    for (int i = 0; i < rect_vec->size - 1; i++) {
        bresenham(rect_vec->data[i].x, rect_vec->data[i].y, rect_vec->data[i + 1].x,
                  rect_vec->data[i + 1].y, bmp, color);
    }
    bresenham(rect_vec->data[rect_vec->size - 1].x, rect_vec->data[rect_vec->size - 1].y,
              rect_vec->data[0].x, rect_vec->data[0].y, bmp, color);
}

void pg_fill(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec) {
    for (int y = rect_vec->data[1].y; y < rect_vec->data[0].y; y++) {
        for (int x = rect_vec->data[1].x; x < rect_vec->data[2].x; x++) {
            bmp->data[y * bmp->width + x] = color;
        }
    }
}

void give_tri(pg_vector_t *tri_vec, double w, double h, double xc, double yc){
    double p1x = xc - h / 2.0;
    double p1y = yc - w / 2.0;
    double p2x = xc + h / 2.0;
    double p2y = yc;
    double p3x = xc - h / 2.0;
    double p3y = yc + w / 2.0;
    double p4x = xc - h / 2.0;
    double p4y = yc;
    pg_append(tri_vec, p4x, p4y);
    pg_append(tri_vec, p3x, p3y);
    pg_append(tri_vec, p2x, p2y);
    pg_append(tri_vec, p1x, p1y);
}

void tri_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *tri_vec) {
    for (int i = 0; i < tri_vec->size - 1; i++){
        bresenham(tri_vec->data[i].x, tri_vec->data[i].y, tri_vec->data[i + 1].x,
                  tri_vec->data[i + 1].y, bmp, color);
    }
    bresenham(tri_vec->data[tri_vec->size - 1].x, tri_vec->data[tri_vec->size - 1].y,
              tri_vec->data[0].x, tri_vec->data[0].y, bmp, color);
}

void tri_fill(bitmap_t *bmp, color_bgr_t color, pg_vector_t *tri_vec, pg_vector_t *fillbuff) {
    int x0[bmp->height];
    int x1[bmp->height];
    for (int i = 0; i < bmp->height; i++) {
        x0[i] = 1000;
        x1[i] = -1;
    }

    for (int k = 0; k < tri_vec->size; k++) {
          printf("%d ", (int)tri_vec->data[k].x);
          printf("%d ", (int)tri_vec->data[k].y);
          printf("%d\n", k);
        }

    for (int j = 0; j < fillbuff->size; j++) {
        if (x0[(int)fillbuff->data[j].y] == 1000) {
            x0[(int)fillbuff->data[j].y] = (int)fillbuff->data[j].x;
            x1[(int)fillbuff->data[j].y] = (int)fillbuff->data[j].x;
        } else {
            if (x0[(int)fillbuff->data[j].y] > (int)fillbuff->data[j].x) {
                x0[(int)fillbuff->data[j].y] = (int)fillbuff->data[j].x;
            }

            if (x0[(int)fillbuff->data[j].y] < (int)fillbuff->data[j].x) {
                x1[(int)fillbuff->data[j].y] = (int)fillbuff->data[j].x;
            }
        }
    }
    
    for (int k = 0; k < bmp->height; k++) {
      if (x0[k] != 1000 && x1[k] != -1 && x0[k] != 0 && x1[k] != 0) {
        bresenham(x0[k], k, x1[k], k, bmp, color);
      }
    }
 }

void rotate(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double angle) {
    double x_pivot = 400.0;
    double y_pivot = 400.0;
    double rotx = 0;
    double roty = 0;
    int i = 0;
    while (i < 4) {
        double x_shifted = rect_vec->data[i].x - x_pivot;
        double y_shifted = rect_vec->data[i].y - y_pivot;
        rotx = x_pivot + (x_shifted * COS(angle) - y_shifted * SIN(angle));
        roty = y_pivot + (x_shifted * SIN(angle) + y_shifted * COS(angle));
        pg_append(transformed_vec, rotx, roty);
        i++;
    }
}
