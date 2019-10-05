#include "bmp.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define SIN(x) sin((x) * M_PI / 180)
#define COS(x) cos((x) * M_PI / 180)

//FUNCTIONS
void pg_create(pg_vector_t *v) {
    v->capacity = 10;
    v->size = 0;
    v->data = malloc(sizeof(pg_points_t *) * v->capacity);
}

void pg_append(pg_vector_t *v, double xvalue, double yvalue) {
  //printf("%ld ", sizeof(pg_points_t *));
  // printf("%f\n", yvalue);
    if (v->capacity == v->size) {
        v->data = realloc(v->data, sizeof(pg_points_t *) * (2 * v->capacity));
        v->capacity *= 2;
    }
    v->data[v->size].x = xvalue;
    v->data[v->size].y = yvalue;
    // printf("%f ", v->data[v->size].x);
    // printf("%f\n", v->data[v->size].y);
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
  // for(int i = 0; i <rect_vec->size; i++) {
  //   printf("%d ", rect_vec->data[i].x);
  //   printf("%d\n", rect_vec->data[i].y);
  // }
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
    for (int i = 0; i < rect_vec->size ; i++) {
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

void tri_fill(bitmap_t *bmp, color_bgr_t color, pg_vector_t *tri_vec) {
      // int x0[bmp->height];
      // int x1[bmp->height];
      // for (int i = 0; i < bmp->height; i++) { //bmp height or minus one?
      //     x0[i] = -1;
      //     x1[i] = -1;
      // }
      // int n = tri_vec->size;
      // for (int i = 0; i < n; i++) {
      //       bresenham((int)tri_vec->data[i % n].x, (int)tri_vec->data[i % n].y,
      //                 (int)tri_vec->data[(i + 1) % n].x,
      //                 (int)tri_vec->data[(i + 1) % n].y, bmp, color);
      //     for (int j = 0; j < n; j++) {
      //           if (x0[(int)tri_vec->data[j].y] == -1) {
      //               x0[(int)tri_vec->data[j].y] = (int)tri_vec->data[j].x;
      //               x1[(int)tri_vec->data[j].y] = (int)tri_vec->data[j].x;
      //         } else {
      //               x0[(int)tri_vec->data[j].y] = fmin(x0[(int)tri_vec->data[j].y],
      //                                                 (int)tri_vec->data[j].x);
      //               x1[(int)tri_vec->data[j].y] = fmax(x0[(int)tri_vec->data[j].y],
      //                                                 (int)tri_vec->data[j].x);
      //         }
      //     }
      // }
      // int y0 = 0;
      // int y1 = 0;
      // int y = 0;
      // while (x0[y] == -1) {
      //         y++;
      //         if (x0[y] != -1) {
      //             break;
      //         }
      // }
      // y0 = y;
      // y = bmp->height - 1;
      //
      // while (x1[y] == -1) {
      //         y--;
      //         if (x1[y] != -1) {
      //             break;
      //         }
      // }
      // y1 = y;
      //
      // int min_x0 = x0[y0];
      // int max_x1 = x1[y1];
      // for (int yf = y0; yf <= y1; yf++) {
      //      for (int xf = x0[yf]; xf <= x1[yf]; xf++) {
      //         //int pix = yf * bmp->width + xf;
      //          bmp->data[yf * bmp->width + xf] = color;
      //      }
      // }
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
        rotx = x_pivot + (x_shifted * COS(angle)
        - y_shifted * SIN(angle));
        roty = y_pivot + (x_shifted * SIN(angle)
        + y_shifted * COS(angle));
        pg_append(transformed_vec, rotx, roty);
        //printf("(%f, %f)\n", transformed_vec->data[i].x, transformed_vec->data[i].y);
        i++;
      }
}
