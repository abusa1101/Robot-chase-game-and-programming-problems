#include "bmp.h"
#include "gx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define M_PI 3.14159265358979323846

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
}

void vector_free(vector_xy_t *v) {
    free(v->data);
    free(v);
}

//GX
void gx_rasterize_line(vector_xy_t *pathpoints, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (1) {
        vector_append(pathpoints, x0, y0);
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

vector_xy_t *call_rasterize(vector_xy_t *points) {
    vector_xy_t *pathpoints = vector_create();
    for (int i = 0; i < points->size - 1; i++) {
        gx_rasterize_line(pathpoints, (int)points->data[i].x, (int)points->data[i].y,
                          (int)points->data[i + 1].x, (int)points->data[i + 1].y);
    }
    gx_rasterize_line(pathpoints, (int)points->data[points->size - 1].x,
                      (int)points->data[points->size - 1].y,
                      (int)points->data[0].x, (int)points->data[0].y);
    vector_free(points);
    return pathpoints;
}

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

void gx_draw(bitmap_t *bmp, color_bgr_t color, vector_xy_t *pathpoints) {
    for (int i = 0; i < pathpoints->size - 1; i++) {
        gx_draw_line(bmp, color, pathpoints->data[i].x, pathpoints->data[i].y,
                     pathpoints->data[i + 1].x, pathpoints->data[i + 1].y);
    }
    gx_draw_line(bmp, color, pathpoints->data[pathpoints->size - 1].x,
                 pathpoints->data[pathpoints->size - 1].y,
                 pathpoints->data[0].x, pathpoints->data[0].y);
    vector_free(pathpoints);
    }

void gx_fill(bitmap_t *bmp, color_bgr_t color, vector_xy_t *pathpoints) {
    int x0[bmp->height];
    int x1[bmp->height];
    for (int i = 0; i < bmp->height; i++) {
        x0[i] = -1;
        x1[i] = -1;
    }
    for (int j = 0; j < pathpoints->size; j++) {
        int x = (int)pathpoints->data[j].x;
        int y = (int)pathpoints->data[j].y;
        if (y < 0 || y >= bmp->height) {
            continue;
        }
        if (x0[y] == -1) {
            x0[y] = x;
            x1[y] = x;
        } else {
            if (x0[y] > x) {
                x0[y] = x;
            }
            if (x0[y] < x) {
                x1[y] = x;
            }
        }
    }
    vector_free(pathpoints);

    for (int y = 0; y < bmp->height; y++) {
        if (x0[y] != -1) {
            int xmin = x0[y] > 0 ? x0[y] : 0;
            int xmax = x1[y] < (bmp->width - 1) ? x1[y] : (bmp->width - 1);
            for (int x = xmin; x <= xmax; x++) {
                bmp->data[y * bmp->width + x] = color;
            }
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

    return points;
}

vector_xy_t *gx_robot(double w, double h) {
    vector_xy_t *points = vector_create();

    vector_append(points, h / 2.0, 0);
    vector_append(points, -h / 2.0, w / 2.0);
    vector_append(points, -h / 2.0, -w / 2.0);

    return points;
}

void gx_rot(double theta, vector_xy_t *points) {
    for (int i = 0; i < points->size; i++) {
        double rot_x = (points->data[i].x * cos(theta) + points->data[i].y * sin(theta));
        double rot_y = (points->data[i].x * -sin(theta) + points->data[i].y * cos(theta));
        points->data[i].x = rot_x;
        points->data[i].y = rot_y;
    }
}

void gx_trans(double x, double y, vector_xy_t *points) {
    for (int i = 0; i < points->size; i++) {
        points->data[i].x += x;
        points->data[i].y += y;
    }
}

void gx_round(vector_xy_t *pathpoints) {
    double epsilon = 1e-6;
    double tempx = pathpoints->data[0].x;
    double tempy = pathpoints->data[0].y;
    for (int i = 0; i < pathpoints->size; i++) {
        if (tempx > pathpoints->data[i].x) {
            tempx = pathpoints->data[i].x;
        }
        if (tempy > pathpoints->data[i].y) {
            tempy = pathpoints->data[i].y;
        }
    }
    for (int i = 0; i < pathpoints->size; i++) {
        if (pathpoints->data[i].x == tempx) {
            pathpoints->data[i].x = ceil(pathpoints->data[i].x);
        } else {
            pathpoints->data[i].x = floor(pathpoints->data[i].x - epsilon);
        }
        if (pathpoints->data[i].y == tempy) {
            pathpoints->data[i].y = ceil(pathpoints->data[i].y);
        } else {
            pathpoints->data[i].y = floor(pathpoints->data[i].y - epsilon);
        }
    }
}

//Objects
void lamp(bitmap_t *bmp, color_bgr_t color, double x, double y) {
    vector_xy_t *points = gx_rect(12, 12);
    gx_rot(M_PI / 4, points);
    gx_trans(x, y, points);
    gx_round(points);
    vector_xy_t *pathpoints = call_rasterize(points);
    gx_fill(bmp, color, pathpoints);
}

void robot(bitmap_t *bmp, color_bgr_t color, double x, double y, double theta) {
    vector_xy_t *points = gx_robot(21, 28);
    gx_rot(theta, points);
    gx_trans(x, y, points);
    gx_round(points);
    vector_xy_t *pathpoints = call_rasterize(points);
    gx_fill(bmp, color, pathpoints);
}

void backdrop(bitmap_t *bmp, color_bgr_t color) {
    vector_xy_t *points = gx_rect(600, 440);
    gx_trans(320, 240, points);
    gx_round(points);
    vector_xy_t *pathpoints = call_rasterize(points);
    gx_draw(bmp, color, pathpoints);
}

//Movement
void gx_update(bitmap_t *bmp, game_t *game, color_bgr_t color_back,
               color_bgr_t color_lamp, color_bgr_t color_robot) {
    gx_clear(bmp);
    backdrop(bmp, color_back);
    lamp(bmp, color_lamp, game->lpos[0].x, game->lpos[0].y);
    lamp(bmp, color_lamp, game->lpos[1].x, game->lpos[1].y);
    lamp(bmp, color_lamp, game->lpos[2].x, game->lpos[2].y);
    robot(bmp, color_robot, game->rpos.x, game->rpos.y, game->rtheta);
}

void gx_clear(bitmap_t *bmp) {
    color_bgr_t color = {0, 0, 0};
    for (int y = 0; y < bmp->height; y++) {
        for (int x = 0; x < bmp->width; x++) {
            bmp->data[y * bmp->width + x] = color;
        }
    }
}

void init_values(game_t *game) {
    game->rpos.x = 320;
    game->rpos.y = 240;
    game->lpos[0].x = 124.1;
    game->lpos[0].y = 224.1;
    game->lpos[1].x = 349.1;
    game->lpos[1].y = 99.1;
    game->lpos[2].x = 449.1;
    game->lpos[2].y = 349.1;
    game->n_lamp = 3;
    game->rtheta = 0;
}

double dotP(double x0, double y0, double x1, double y1) {
    double product = x0 * x1;
    product = product + y0 * y1;
    return product;
}

void activateMove(game_t *game) {
    double move_l = 0;
    double move_r = 0;
    double max_movement = 12;
    double wheel_base = 80;
    double lamp_power = 100000;
    for (int i = 0; i < game->n_lamp; i++) {
        double dist_sq = pow(game->lpos[i].x - game->rpos.x, 2) +
                         pow(game->lpos[i].y - game->rpos.y, 2);
        double dir_x = (game->lpos[i].x - game->rpos.x) / sqrt(dist_sq);
        double dir_y = (game->lpos[i].y - game->rpos.y) / sqrt(dist_sq);
        double eye_l_x = cos(game->rtheta + (M_PI / 3));
        double eye_l_y = -sin(game->rtheta + (M_PI / 3));
        double eye_r_x = cos(game->rtheta - (M_PI / 3));
        double eye_r_y = -sin(game->rtheta - (M_PI / 3));
        move_l += fmax(0.0, dotP(dir_x, dir_y, eye_r_x, eye_r_y)) * lamp_power / (dist_sq);
        move_r += fmax(0.0, dotP(dir_x, dir_y, eye_l_x, eye_l_y)) * lamp_power / (dist_sq);
    }
    move_l = fmin(max_movement, move_l);
    move_r = fmin(max_movement, move_r);
    game->rtheta += (move_r - move_l) / (wheel_base);
    double fwd_dist = (move_r + move_l) / 2;
    double xdist = fwd_dist * cos(game->rtheta);
    double ydist = fwd_dist * -sin(game->rtheta);
    game->rpos.x += xdist;
    game->rpos.y += ydist;
}

bool robot_collision(game_t *game, vector_xy_t *lamp) {
    vector_xy_t *robot = gx_robot(21, 28);
    gx_rot(game->rtheta, robot);
    gx_trans(game->rpos.x, game->rpos.y, robot);
    bool collides = pg_collision(robot, lamp);
    vector_free(robot);
    return collides;
}

void resolve_collision(game_t *game, double lamp_x, double lamp_y) {
    vector_xy_t *lamp = gx_rect(12, 12);
    gx_rot(M_PI / 4, lamp);
    gx_trans(lamp_x, lamp_y, lamp);
    while (robot_collision(game, lamp)) {
        double dx = game->rpos.x - lamp_x;
        double dy = game->rpos.y - lamp_y;
        double dist_sq = pow(dx, 2) + pow(dy, 2);
        game->rpos.x += 0.5 * dx / dist_sq;
        game->rpos.y += 0.5 * dy / dist_sq;
    }
    vector_free(lamp);
}

int pg_collision(vector_xy_t *pg1, vector_xy_t *pg2) {
    if ((pg_intersection(pg1, pg2) == 1) ||
        (check4containment(pg2, pg1->data[0].x, pg1->data[0].y) == 1) ||
        (check4containment(pg1, pg2->data[0].x, pg2->data[0].y) == 1)) {
        return 1;
    }
    return 0;
}

bool pg_intersection(vector_xy_t *pg1, vector_xy_t *pg2) {
    for (int i = 0; i < pg1->size; i++) {
        int i2 = (i + 1) % pg1->size;
        double x0 = pg1->data[i].x;
        double y0 = pg1->data[i].y;
        double x1 = pg1->data[i2].x;
        double y1 = pg1->data[i2].y;
        for (int j = 0; j < pg2->size; j++) {
            int j2 = (j + 1) % pg2->size;
            double x2 = pg2->data[j].x;
            double y2 = pg2->data[j].y;
            double x3 = pg2->data[j2].x;
            double y3 = pg2->data[j2].y;
            if (line_intersection(x0, y0, x1, y1, x2, y2, x3, y3)) {
                return true;
            }
        }
    }
    return false;
}

bool line_intersection(double x0, double y0, double x1, double y1,
                       double x2, double y2, double x3, double y3) {
    double cp0 = ((x3 - x2) * (y0 - y2)) - ((x0 - x2) * (y3 - y2)); //cp for p0
    double cp1 = ((x3 - x2) * (y1 - y2)) - ((x1 - x2) * (y3 - y2)); //cp for p1
    double check_cp01 = cp0 * cp1;

    double cp2 = ((x1 - x0) * (y2 - y0)) - ((x2 - x0) * (y1 - y0)); //cp for p2
    double cp3 = ((x1 - x0) * (y3 - y0)) - ((x3 - x0) * (y1 - y0)); //cp for p3
    double check_cp23 = cp2 * cp3;

    return check_cp01 <= 0 && check_cp23 <= 0 && (check_cp01 != 0 || check_cp23 != 0);
}

bool check4containment(vector_xy_t *pg, double x, double y) {
    int counter1 = 0;
    int counter2 = 0;
    for (int i = 0; i < pg->size; i++) {
        int i2 = (i + 1) % pg->size;
        double cp = ((y - pg->data[i].y) * (pg->data[i2].x - pg->data[i].x))
                     - ((x - pg->data[i].x) * (pg->data[i2].y - pg->data[i].y));
        if (cp > 0) {
            counter1++;
        } else if (cp < 0) {
            counter2++;
        }
    }
    return counter1 == 0 || counter2 == 0;
}
