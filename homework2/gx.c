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
void pg_create(pg_vector_t *v) {
    v->capacity = 400;
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
}/;

void pg_free(pg_vector_t *v) {
    free(v->data);
}

//GX

vector_xy_i32_t *gx_rasterize_line(int x0, int y0, int x1, int y1);

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
    for (int i = 0; i < tri_vec->size - 1; i++) {
        gx_draw_line(tri_vec->data[i].x, tri_vec->data[i].y, tri_vec->data[i + 1].x,
                  tri_vec->data[i + 1].y, bmp, color);
    }
    gx_draw_line(tri_vec->data[tri_vec->size - 1].x, tri_vec->data[tri_vec->size - 1].y,
              tri_vec->data[0].x, tri_vec->data[0].y, bmp, color);
}

void gx_fill(bitmap_t *bmp, color_bgr_t color, vector_xy_t *points) {
    int x0[bmp->height];
    int x1[bmp->height];
    for (int i = 0; i < bmp->height; i++) {
        x0[i] = 1000;
        x1[i] = -1;
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

void gx_clear(bitmap_t *bmp) {
    color_bgr_t color = {0, 0, 0};
    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 680; x++) {
            bmp->data[y * bmp->width + x] = color;
        }
    }
}

vector_xy_t *gx_rect(double width, double height) {
    pg_vector_t poly_vec = {0};
    pg_create(&poly_vec);
    double xc = 100;
    double yc = 100;
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
    pg_append(poly_vec, p4x, p4y);
    pg_append(poly_vec, p3x, p3y);
    pg_append(poly_vec, p2x, p2y);
    pg_append(poly_vec, p1x, p1y);

    return &poly_vec;
}

vector_xy_t *gx_lamp(double width, double height) {
    vector_xy_t* vertices = gx_rect(12, 12);

}

vector_xy_t *gx_robot(double dim);

vector_xy_t *gx_rot(double theta, vector_xy_t *points, double xpivot, double ypivot) {
    double rotated_x = 0;
    double rotated_y = 0;
    int i = 0;
    while (i < 4) {
        double x_shifted = points->data[i].x - x_pivot;
        double y_shifted = points->data[i].y - y_pivot;
        rotated_x = x_pivot + (x_shifted * COS(angle) - y_shifted * SIN(angle));
        rotated_y = y_pivot + (x_shifted * SIN(angle) + y_shifted * COS(angle));
        //pg_append(transformed_vec, rotx, roty);
        points->data[i].x = rotated_x;
        points->data[i].y = rotated_y;
        i++;
    }
    return points;
}

vector_xy_t *gx_trans(double x, double y, vector_xy_t *points) {
    for (int i = 0; i < points->size; i++) {
        double transformed_x = points->data[i].x + xglobal;
        double transformed_y = points->data[i].y + yglobal;
        //pg_append(transformed_vec, transx, transy);
        points->data[i].x = transformed_x;
        points->data[i].y = transformed_y;
    }
    return points;
}

vector_xy_t *gx_round(vector_xy_t *points) {
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
    return points;
}
