#include "bmp.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define SIN(x) sin((x)*M_PI / 180)
#define COS(x) cos((x)*M_PI / 180)

// Vector Operations
vector_xy_t *vector_create(void) {
    vector_xy_t *v = malloc(sizeof(vector_xy_t));
    v->capacity = 8;
    v->size = 0;
    v->data = malloc(sizeof(points_t) * v->capacity);
    return v;
}

void vector_append(vector_xy_t *v, double xvalue, double yvalue) {
    if (v->capacity == v->size) {
        v->data = realloc(v->data, sizeof(points_t) * (2 * v->capacity));
        v->capacity *= 2;
    }
    v->data[v->size].x = xvalue;
    v->data[v->size].y = yvalue;
    v->size++;
}/;

void vector_free(vector_xy_t *v) {
    free(v->data);
    free(v);
}

//GX
//vector_xy_i32_t *gx_rasterize_line(int x0, int y0, int x1, int y1);

void gx_draw_line(bitmap_t *bmp, color_bgr_t color, int x0, int y0, int x1, int y1) {
  int dx = abs(x1 - x0);
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

void gx_draw(bitmap_t *bmp, color_bgr_t color, vector_xy_t *points) {
    for (int i = 0; i < points->size - 1; i++) {
        gx_draw_line(bmp, color, points->data[i].x, points->data[i].y, points->data[i + 1].x,
                  points->data[i + 1].y);
    }
    gx_draw_line(bmp, color, tri_vec->data[points->size - 1].x, points->data[points->size - 1].y,
              points->data[0].x, points->data[0].y);
}

void gx_fill(bitmap_t *bmp, color_bgr_t color, vector_xy_t *points) {
    int x0[bmp->height];
    int x1[bmp->height];
    for (int i = 0; i < bmp->height; i++) {
        x0[i] = 1000;
        x1[i] = -1;
    }
    for (int j = 0; j < points->size; j++) {
        if (x0[(int)points->data[j].y] == 1000) {
            x0[(int)points->data[j].y] = (int)points->data[j].x;
            x1[(int)points->data[j].y] = (int)points->data[j].x;
        } else {
            if (x0[(int)points->data[j].y] > (int)points->data[j].x) {
                x0[(int)points->data[j].y] = (int)points->data[j].x;
            }

            if (x0[(int)points->data[j].y] < (int)points->data[j].x) {
                x1[(int)points->data[j].y] = (int)points->data[j].x;
            }
        }
    }
    for (int k = 0; k < bmp->height; k++) {
        if (x0[k] != 1000 && x1[k] != -1 && x0[k] != 0 && x1[k] != 0) {
            gx_draw_line(bmp, color, x0[k], k, x1[k], k);
        }
    }
}

void gx_clear(bitmap_t *bmp) {
    color_bgr_t color = {0, 0, 0};
    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 680; x++) {
            bmp->data[y * bmp->width + x] = color;
        }
    }
}

vector_xy_t *gx_rect(double width, double height) {
    vector_xy_t *points = vector_create();
    double xc = 0;
    double yc = 0;
    double p1x = xc + width / 2.0;
    double p1y = yc + height / 2.0;

    double p2x = xc + width / 2.0;
    double p2y = yc - height / 2.0;

    double p3x = xc - width / 2.0;
    double p3y = yc - height / 2.0;

    double p4x = xc - width / 2.0;
    double p4y = yc + height / 2.0;

    vector_append(points, p4x, p4y);
    vector_append(points, p3x, p3y);
    vector_append(points, p2x, p2y);
    vector_append(points, p1x, p1y);

    return &points;
}

void gx_rot(double theta, vector_xy_t *points) {
    int i = 0;
    while (i < 4) {
        points->data[i].x = (points->data[i].x * COS(theta) - points->data[i].y * SIN(theta));
        points->data[i].y = (points->data[i].x * SIN(theta) + points->data[i].y * COS(theta));
        i++;
    }
    return points;
}

void gx_trans(double x, double y, vector_xy_t *points) {
    for (int i = 0; i < points->size; i++) {
        points->data[i].x += x;
        points->data[i].y += y;
    }
}

void gx_round(vector_xy_t *points) {
  double epsilon = 1e-6;
  double tempx = points->data[0].x;
  double tempy = points->data[0].y;
  for (int i = 0; i < points->size; i++) {
      if (tempx > points->data[i].x) {
          tempx = points->data[i].x;
      }
      if (tempy > points->data[i].y) {
          tempy = points->data[i].y;
      }
  }
  for (int i = 0; i < points->size; i++) { //check for min value and round off
      if (points->data[i].x == tempx) {
          points->data[i].x = ceil(points->data[i].x);
      } else {
          points->data[i].x = floor(points->data[i].x - epsilon);
      }
      if (points->data[i].y == tempy) {
          points->data[i].y = ceil(points->data[i].y);
      } else {
          points->data[i].y = floor(points->data[i].y - epsilon);
      }
  }
}

//Objects
vector_xy_t *gx_lamp(bitmap_t *bmp, color_bgr_t color, double x, double y) {
  double width = 12;
  double height = 12;
  vector_xy_t* points1 = gx_rect(12, 12);
  gx_rot(45, points1);
  gx_trans(x, y, points1);
  gx_round(points);
  gx_fill(bmp, color, points);

}

vector_xy_t *gx_robot(bitmap_t *bmp, color_bgr_t color) {
  double width = 12;
  double height = 12;
  vector_xy_t* points1 = gx_rob_points(12, 12);
  gx_rot(0, points1);
  gx_trans(320, 240, points1);
  gx_round(points);
  gx_fill(bmp, color, points);
