#include "bmp.h"
#include "movement.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define SIN(x) sin((x)*M_PI / 180)
#define COS(x) cos((x)*M_PI / 180)

//initialize
void initGraphics(bitmap_t bmp, color_bgr_t color, color_bgr_t color_robot, color_bgr_t color_lamp,
                  game_t *game,
                  pg_vector_t rect_vec, pg_vector_t trans1_vec, pg_vector_t fillbuff1,
                  pg_vector_t trans2_vec, pg_vector_t fillbuff2, pg_vector_t trans3_vec,
                  pg_vector_t fillbuff3, pg_vector_t trans4_vec, pg_vector_t fillbuff4) {
    //, game_t *game, ???
    //border
    give_rect(&rect_vec, 600.0, 440.0, 320, 240);
    cd2pixel(&rect_vec);
    pg_draw(&bmp, color, &rect_vec, 0);

    //robot
    give_tri(&trans4_vec, robot.width, robot.length, robot.location.x, robot.location.y); //21 28 320 240
    cd2pixel(&trans4_vec);
    tri_draw(&bmp, color_robot, &trans4_vec);
    callb2(&fillbuff4, &trans4_vec);
    tri_fill(&bmp, color_robot, &trans4_vec, &fillbuff4);

    //lamps
    lamp1.size = 12;
    lamp1.rot = 45;
    lamp1.location.x = 124.1;
    lamp1.location.y = 224.1;
    give_rect(&l1_vec, lamp1.size, lamp1.size, lamp1.location.x, lamp1.location.y);
    rotate2(&l1_vec, &trans1_vec, lamp1.rot, lamp1.location.x, lamp1.location.y);
    cd2pixel(&trans1_vec);
    pg_draw(&bmp, color_lamp, &trans1_vec, 0);
    callb2(&fillbuff1, &trans1_vec);
    tri_fill(&bmp, color_lamp, &trans1_vec, &fillbuff1);

    lamp2.location.x = 349.1;
    lamp2.location.y = 99.1;
    give_rect(&l2_vec, lamp1.size, lamp1.size, lamp2.location.x, lamp2.location.y);
    rotate2(&l2_vec, &trans2_vec, lamp1.rot, lamp2.location.x, lamp2.location.y);
    cd2pixel(&trans2_vec);
    pg_draw(&bmp, color_lamp, &trans2_vec, 0);
    callb2(&fillbuff2, &trans2_vec);
    tri_fill(&bmp, color_lamp, &trans2_vec, &fillbuff2);

    lamp3.location.x = 449.1;
    lamp3.location.y = 349.1;
    give_rect(&l3_vec, lamp1.size, lamp1.size, lamp3.location.x, lamp3.location.y);
    rotate2(&l3_vec, &trans3_vec, lamp1.rot, lamp3.location.x, lamp3.location.y);
    cd2pixel(&trans3_vec);
    pg_draw(&bmp, color_lamp, &trans3_vec, 0);
    callb2(&fillbuff3, &trans3_vec);
    tri_fill(&bmp, color_lamp, &trans3_vec, &fillbuff3);
}

//Movement
double dotP(double x0, double y0, double x1, double y1) {
  double product = x0 * x1;
  product = product + y0 * y1;
  return product;
}

void activateMove(game_t *game) {
    double move_l = 0;
    double move_r = 0;
    double max_movement = 12;
    double theta = 0;
    double wheel_base = 80;
    double lamp_power = 100000;

    for (int i = 0; i < 3; i++) {
        double dist_sq = pow(game.lamp[i].location.x - game.robot.location.x, 2) + pow(game.lamp[i].location.y - game.robot.location.y, 2);
        double dir_x = (game.lamp[i].location.x - game.robot.location.x)/ sqrt(dist_sq);
        double dir_y = (game.lamp[i].location.y - game.robot.location.y)/ sqrt(dist_sq);
        double eye_l_x = (game.lamp[i].location.x - game.robot.location.x) / cos((theta + 60)* M_PI / 180);
        double eye_l_y = (game.lamp[i].location.y - game.robot.location.y) / cos((theta + 60)* M_PI / 180);
        double eye_r_x  = (game.lamp[i].location.x - game.robot.location.x) / cos((theta - 60)* M_PI / 180);
        double eye_r_y  = (game.lamp[i].location.x - game.robot.location.x) / cos((theta - 60)* M_PI / 180);
        move_l += max(0.0, dotP(dir_x, dir_y, eye_l_x, eye_l_y) * lamp_power / (dist_sq);
        move_r += max(0.0, dotP(dir_x, dir_y, eye_r_x, eye_r_y) * lamp_power / (dist_sq);

        move_l = min(max_movement, move_l);
        move_r = min(max_movement, move_r);
        theta += (move_r - move_l) / (wheel_base);
        fwd_dist = (move_r + move_l) / 2;
        double xdist = fwd_dist * cos(theta);
        double ydist = fwd_dist * sin(theta);
        game.robot.location.x += xdist;
        game.robot.location.y += ydist;
    }
    //actually move the robot using graphics
}

int check4collision(robot_t *robot, lamp_t *lamp) {
    //all collision funcs here
}

void retreat(robot_t *robot, lamp_t *lamp) {
    int flag = check4collision(robot, lamp);
    while (flag == 1) {
        robot.location.x += 0.5;
        flag = check4collision();
    }
}

void clearGraphics(bitmap_t *bmp) {
    color_bgr_t color = {0, 0, 0};
    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 680; x++) {
            bmp->data[y * bmp->width + x] = color;
        }
    }
}

void updateGraphics() {
    clearGraphics(bmp);

    //border
    give_rect(&rect_vec, 600.0, 440.0, 320, 240);
    cd2pixel(&rect_vec);
    pg_draw(&bmp, color, &rect_vec, 0);

    //robot
    give_tri(&trans4_vec, robot.width, robot.length, robot.location.x, robot.location.y); //21 28 320 240
    cd2pixel(&trans4_vec);
    tri_draw(&bmp, color_robot, &trans4_vec);
    callb2(&fillbuff4, &trans4_vec);
    tri_fill(&bmp, color_robot, &trans4_vec, &fillbuff4);

    //lamps
    lamp1.size = 12;
    lamp1.rot = 45;
    lamp1.location.x = 124.1;
    lamp1.location.y = 224.1;
    give_rect(&l1_vec, lamp1.size, lamp1.size, lamp1.location.x, lamp1.location.y);
    rotate2(&l1_vec, &trans1_vec, lamp1.rot, lamp1.location.x, lamp1.location.y);
    cd2pixel(&trans1_vec);
    pg_draw(&bmp, color_lamp, &trans1_vec, 0);
    callb2(&fillbuff1, &trans1_vec);
    tri_fill(&bmp, color_lamp, &trans1_vec, &fillbuff1);

    lamp2.location.x = 349.1;
    lamp2.location.y = 99.1;
    give_rect(&l2_vec, lamp1.size, lamp1.size, lamp2.location.x, lamp2.location.y);
    rotate2(&l2_vec, &trans2_vec, lamp1.rot, lamp2.location.x, lamp2.location.y);
    cd2pixel(&trans2_vec);
    pg_draw(&bmp, color_lamp, &trans2_vec, 0);
    callb2(&fillbuff2, &trans2_vec);
    tri_fill(&bmp, color_lamp, &trans2_vec, &fillbuff2);

    lamp3.location.x = 449.1;
    lamp3.location.y = 349.1;
    give_rect(&l3_vec, lamp1.size, lamp1.size, lamp3.location.x, lamp3.location.y);
    rotate2(&l3_vec, &trans3_vec, lamp1.rot, lamp3.location.x, lamp3.location.y);
    cd2pixel(&trans3_vec);
    pg_draw(&bmp, color_lamp, &trans3_vec, 0);
    callb2(&fillbuff3, &trans3_vec);
    tri_fill(&bmp, color_lamp, &trans3_vec, &fillbuff3);

}


//Vector Operations
void pg_create(pg_vector_t *v) {
    v->capacity = 10;
    v->size = 0;
    v->data = malloc(sizeof(pg_points_t *) * v->capacity);
}

void pg_create2(pg_vector_t *v) {
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
}

void pg_free(pg_vector_t *v) {
    free(v->data);
}

void bresenham(int x0, int y0, int x1, int y1, bitmap_t *bmp, color_bgr_t color) {
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

void bresenham2(pg_vector_t *fillbuff, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
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
    bresenham2(fillbuff, (int)transformed_vec->data[transformed_vec->size - 1].x,
               (int)transformed_vec->data[transformed_vec->size - 1].y,
               (int)transformed_vec->data[0].x, (int)transformed_vec->data[0].y);
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

void give_tri(pg_vector_t *tri_vec, double w, double h, double xc, double yc) {
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
    for (int i = 0; i < tri_vec->size - 1; i++) {
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

void rotate2(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double angle,
             double x_pivot, double y_pivot) {
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
