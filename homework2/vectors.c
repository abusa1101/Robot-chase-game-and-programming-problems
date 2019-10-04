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
  pg_append(rect_vec,p4x, p4y);
  pg_append(rect_vec,p3x, p3y);
  pg_append(rect_vec,p2x, p2y);
  pg_append(rect_vec,p1x, p1y);
  // for(int i = 0; i <rect_vec->size; i++) {
  //   printf("%d ", rect_vec->data[i].x);
  //   printf("%d\n", rect_vec->data[i].y);
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

void pg_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec, int n) {
  if (n == 3) {
    bresenham(rect_vec->data[2].x, rect_vec->data[2].y, rect_vec->data[3].x, rect_vec->data[3].y, bmp, color);
    bresenham(rect_vec->data[rect_vec->size - 1].x, rect_vec->data[rect_vec->size - 1].y, rect_vec->data[0].x, rect_vec->data[0].y, bmp, color);
    return;
  }
  for(int i = 0; i < rect_vec->size - 1; i++){
    bresenham(rect_vec->data[i].x, rect_vec->data[i].y, rect_vec->data[i+1].x, rect_vec->data[i+1].y, bmp, color);
  }
  bresenham(rect_vec->data[rect_vec->size - 1].x, rect_vec->data[rect_vec->size - 1].y, rect_vec->data[0].x, rect_vec->data[0].y, bmp, color);
}

void pg_fill(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec) {
  for (int y = rect_vec->data[1].y; y <rect_vec->data[0].y; y++) {
    for (int x = rect_vec->data[1].x; x <rect_vec->data[2].x; x++) {
      bmp->data[y * bmp->width + x] = color;
    }
  }

  // //for each line in polygon, represented by index i that the line starts on
  // for (int i = 0; i <bmp->height - 1; i++) {
  //   i2 = (i + 1) % n; // index of second point of the line
  //   points = rasterize_line(point at i, point at i2);
  //   for (int i = 0; i <bmp->height - 1; i++) {
  //     x0[i] = -1;
  //     x1[i] = -1;
  //   }
  //   for point (x, y) in points {
  //     // follow logic in homework2 document to modify x0[y] and x1[y]
  //     for (int y = 0; y <bmp->height - 1; y++) {
  //       if (x0[y] == -1) {
  //         x0[y] = x;
  //         x1[y] = x;
  //       } else {
  //         x0[y] = min(x0[y], x);
  //         x1[y] = max(x1[y], x);
  //       }
  //     }
  //   }
  // }
}

void give_tri(pg_vector_t *tri_vec, double w, double h, double xc, double yc){
  double p1x = xc - h/ 2.0;
  double p1y = yc - w / 2.0;
  double p2x = xc + h / 2.0;
  double p2y = yc;
  double p3x = xc - h / 2.0;
  double p3y = yc + w / 2.0;
  double p4x = xc - h / 2.0;
  double p4y = yc;
  pg_append(tri_vec,p4x, p4y);
  pg_append(tri_vec,p3x, p3y);
  pg_append(tri_vec,p2x, p2y);
  pg_append(tri_vec,p1x, p1y);
  for(int i = 0; i < tri_vec->size; i++){
    printf("%f ", tri_vec->data[i].x);
    printf("%f\n", tri_vec->data[i].y);
  }
}

void tri_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *tri_vec) {
  for(int i = 0; i < tri_vec->size - 1; i++){
    bresenham(tri_vec->data[i].x, tri_vec->data[i].y, tri_vec->data[i+1].x, tri_vec->data[i+1].y, bmp, color);
  }
  bresenham(tri_vec->data[tri_vec->size - 1].x, tri_vec->data[tri_vec->size - 1].y, tri_vec->data[0].x, tri_vec->data[0].y, bmp, color);
}

void tri_fill(bitmap_t *bmp, color_bgr_t color, pg_vector_t *tri_vec) {
  //   for (int y = tri_vec->data[1].y; y <tri_vec->data[0].y; y++) {
  //     for (int x = tri_vec->data[1].x; x <tri_vec->data[2].x; x++) {
  //       bmp->data[y * bmp->width + x] = color;
  //     }
  // }

  int x0[bmp->height];
  //int x1[bmp->height];
  int val = tri_vec->data[3].y;
  for (int i = 0; i <21; i++) {
    x0[i] = val;
    // x1[i] = 28;
    printf("%d ", x0[i]);
    for (int y = tri_vec->data[3].y; y <tri_vec->data[1].y; y++) {
      bmp->data[y * bmp->width + x0[i]] = color;
    }
    val++;
  }


}

void rotate(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double radrot) {
  double xpivot = 0.0; //rotation pivot point
  double ypivot = 0.0;
  for(int i = 0; i < rect_vec->size - 1; i++) {
    double rotx = xpivot + ((rect_vec->data[i].x - xpivot) * cos(radrot) - rect_vec->data[i].x - ypivot * sin(radrot));
    double roty = ypivot + ((rect_vec->data[i].y - xpivot) * sin(radrot) + rect_vec->data[i].y - ypivot * cos(radrot));
    pg_append(transformed_vec,rotx, roty);
    // printf("%d ", transformed_vec->data[i].x);
    // printf("%d\n", transformed_vec->data[i].y);
  }
}
