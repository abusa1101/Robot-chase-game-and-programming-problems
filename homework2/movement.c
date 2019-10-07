#include "bmp.h"
#include "movement.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define SIN(x) sin((x)*M_PI / 180)
#define COS(x) cos((x)*M_PI / 180)

//Movement
// void setConstants(robot_t robot, lamp_t lamp1, lamp_t lamp2, lamp_t lamp3) {
//   // game.color.b = 255; //color_bgr_t color_robot =
//   // game.color.g = 255;
//   // game.color.r = 255;
//   // robot.color.b = 0;
//   // robot.color.g = 255;
//   // robot.color.r = 0;
//   robot.location.data->x = 320;
//   robot.location.data->y = 240;
//   robot.max_movement = 12; //max pixels mvmt in a timestep
//   robot.wheel_base = 80;
//   robot.width = 21;
//   robot.length = 28;
//   // lamp1.color.b = 0;
//   // lamp1.color.g = 255;
//   // lamp1.color.r = 255;
//   // lamp2.color.b = 0;
//   // lamp2.color.g = 255;
//   // lamp2.color.r = 255;
//   // lamp3.color.b = 0;
//   // lamp3.color.g = 255;
//   // lamp3.color.r = 255;
//   lamp1.power = 100000;
//   lamp2.power = 100000;
//   lamp3.power = 100000;
//   lamp1.size = 12;
//   lamp2.size = 12;
//   lamp3.size = 12;
//   lamp1.rot = 45;
//   lamp2.rot = 45;
//   lamp3.rot = 45;
//   lamp1.location.data->x = 124.1;
//   lamp1.location.data->y = 224.1;
//   lamp2.location.data->x = 349.1;
//   lamp2.location.data->y = 99.1;
//   lamp3.location.data->x = 449.1;
//   lamp3.location.data->y = 349.1;
// }

// double dotP(game_t game) {
//   double product = 0;
//   for (int i = 0; i < 2; i++) { //2 because 2D ai + bj
//     product = product + dir[i] * eye[i];
//   }
//   return product;
// } //LOOK AT THIS
//
// void rotateP(double dir, double angle, double x_pivot, double y_pivot) {
//   double rotx = 0;
//   double roty = 0;
//   // int i = 0;
//   // while (i < 2) {
//   double x_shifted = dir[i] - x_pivot;
//   double y_shifted = dir[i] - y_pivot;
//   rotx = x_pivot + (x_shifted * COS(angle) - y_shifted * SIN(angle));
//   roty = y_pivot + (x_shifted * SIN(angle) + y_shifted * COS(angle));
//   //    pg_append(transformed_vec, rotx, roty);
//   //     i++;
//   // }
// }
//
// void activateMove(game_t game) {
//   game.robot.move_l = 0;
//   game.robot.move_r = 0;
//   for (int i = 0; i < game.n_lamps; i++) {
//     game.robot.dist_sq = pow(game.lamp.location.x - game.robot.location.x, 2) + pow(game.lamp.location.y - game.robot.location.y, 2);
//     game.robot.dir.x = (game.lamp.location.x - game.robot.location.x)/ sqrt(game.robot.dist_sq);
//     game.robot.dir.y = (game.lamp.location.y - game.robot.location.y)/ sqrt(game.robot.dist_sq);
//     game.robot.eye_l.x = (game.lamp.location.x - game.robot.location.x) / cos((game.robot.theta + 60)* M_PI / 180);
//     game.robot.eye_l.y = (game.lamp.location.y - game.robot.location.y) / cos((game.robot.theta + 60)* M_PI / 180);
//     game.robot.eye_r.x  = (game.lamp.location.x - game.robot.location.x) / cos((game.robot.theta - 60)* M_PI / 180);
//     game.robot.eye_r.y  = (game.lamp.location.x - game.robot.location.x) / cos((game.robot.theta - 60)* M_PI / 180);
//     game.robot.move_l += max(0.0, dotP(game.robot.dir, game.robot.eye_l) * game.lamp.power / (game.robot.dist_sq);
//     game.robot.move_r += max(0.0, dotP(game.robot.dir, game.robot.eye_r) * game.lamp.power / (game.robot.dist_sq);
//
//     game.robot.move_l = min(game.robot.max_movement, game.robot.move_l);
//     game.robot.move_r = min(game.robot.max_movement, game.robot.move_r);
//     game.robot.theta += (game.robot.move_r - game.robot.move_l) / (game.robot.wheel_base);
//     game.robot.fwd_dist = (game.robot.move_r + game.robot.move_l) / 2;
//     double xdist = game.robot.fwd_dist * cos(game.robot.theta);
//     double ydist = game.robot.fwd_dist * sin(game.robot.theta);
//     game.robot.location.x += xdist;
//     game.robot.location.y += ydist;
//   }
//     //actually move the robot using graphics
// }

//Graphics
// void initGraphics(game_t game, lamp_t lamp1, lamp_t lamp2, lamp_t lamp3, pg_points_t *rect_vec) {
//   //border
//   give_rect(&rect_vec, 600.0, 440.0, 320.0, 240.0);
//   cd2pixel(&rect_vec);
//   pg_draw(&bmp, game.color, &rect_vec, 0);
//
//   //Lamp 1
//   give_rect(&l1_vec, lamp1.size, lamp1.size, lamp1.location.data->x, lamp1.location.data->y);
//   rotate2(&l1_vec, &trans1_vec, lamp1.rot, lamp1.location.data->x, lamp1.location.data->y);
//   cd2pixel(&trans1_vec);
//   pg_draw(&bmp, lamp1.color, &trans1_vec, 0);
//   //callb2(&fillbuff1, &trans1_vec);
//   //tri_fill(&bmp, color_lamp, &trans1_vec, &fillbuff1);
//
//   //Lamp 2
//   give_rect(&l1_vec, lamp2.size, lamp2.size, lamp2.location.data->x, lamp2.location.data->y);
//   rotate2(&l1_vec, &trans1_vec, lamp2.rot, lamp2.location.data->x, lamp2.location.data->y);
//   cd2pixel(&trans1_vec);
//   pg_draw(&bmp, lamp2.color, &trans1_vec, 0);
//
//   //Lamp 3
//   give_rect(&l1_vec, lamp3.size, lamp3.size, lamp3.location.data->x, lamp3.location.data->y);
//   rotate2(&l1_vec, &trans1_vec, lamp3.rot, lamp3.location.data->x, lamp3.location.data->y);
//   cd2pixel(&trans1_vec);
//   pg_draw(&bmp, lamp3.color, &trans1_vec, 0);
//
//   //robot
//   give_tri(&rob_vec, game.robot.width, game.robot.length, 320, 240);
//   cd2pixel(&rob_vec);
//   tri_draw(&bmp, game.robot.color, &rob_vec);
//   callb2(&fillbuff, &rob_vec);
//   tri_fill(&bmp, game.robot.color, &rob_vec, &fillbuff);
//
// }

// void resetGraphics (game_t game, pg_points_t *rect_vec) {
//   //border
//   double xcb = 320.0;
//   double ycb = 240.0;
//   give_rect(&rect_vec, 600.0, 440.0, xcb, ycb);
//   cd2pixel(&rect_vec);
//   pg_draw(&bmp, color, &rect_vec, 0);
//   //lamps
//   double lamp_size = 12.0;
//   double xcl1 = 124.1;
//   double ycl1 = 224.1;
//   give_rect(&l1_vec, lamp_size, lamp_size, xcl1, ycl1);
//   rotate2(&l1_vec, &trans1_vec, 45, xcl1, ycl1);
//   cd2pixel(&trans1_vec);
//   pg_draw(&bmp, color_lamp, &trans1_vec, 0);
//   callb2(&fillbuff1, &trans1_vec);
//   tri_fill(&bmp, color_lamp, &trans1_vec, &fillbuff1);
// }
//
// void updateGraphics(game_t game, pg_points_t *rect_vec) {
//
//   resetGraphics(game_t game, pg_points_t *rect_vec);
//   //Change robot position
//   give_tri(&rob_vec, 21.0, 28.0, 320, 240); //get this point from polygon
//   cd2pixel(&rob_vec);
//   tri_draw(&bmp, color, &rob_vec);
//   callb2(&fillbuff, &rob_vec);
//   tri_fill(&bmp, color_robot, &rob_vec, &fillbuff);
// }
//
// //Collision
// int line_intersection(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3) {
//   double cp0 = (x3 - x2) * (y0 - y2) - (x0 - x2) * (y3 - y2); //cp for p0
//   double cp1 = (x3 - x2) * (y1 - y2) - (x1 - x2) * (y3 - y2); //cp for p1
//   double cp2 = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0); //cp for p2
//   double cp3 = (x1 - x0) * (y3 - y0) - (x3 - x0) - (y1 - y0); //cp for p3
//
//   double check_cp23 = cp2 * cp3;
//   double check_cp01 = cp0 * cp1;
//
//   if(check_cp01 <= 0 && check_cp23 <= 0) {
//     if ((check_cp01 != 0 || check_cp23 != 0)) {
//       return 1;
//     }
//   } else {
//     return 0;
//   }
// }
//
// int pg_intersection(polygon_t *pg1, polygon_t *pg2) {
//   for (int i = 0; i < pg1->npoints; i++) {
//     int i2 = (i + 1) % pg->npoints; //check hw2 document for this indexing reference
//     double x0 = pg1->x[i];
//     double y0 = pg1->y[i];
//     double x1 = pg1->x[i2];
//     double y1 = pg1->y[i2];
//     for (int j = 0; j < pg2->npoints; j++) {
//       int j2 = (j + 1) % pg2-> npoints;
//       double x2 = pg2->x[j];
//       double y2 = pg2->x[j];
//       double x3 = pg2->x[j2];
//       double y3 = pg2->x[j2];
//       if (line_intersection(x0, y0, x1, y1, x2, y2, x3, y3) == 1) {
//         return 1;
//       } else {
//         return 0;
//       }
//     }
//   }
// }
//
// int pg_collision(polygon_t *pg1, polygon_t *pg2) {
//   if (pg_intersection(pg1,pg2) == 1) {
//     return 1;
//   } else if (check4containment(pg1, pg2->x[0], pg2->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
//     return 1;
//   } else if (check4containment(pg2, pg1->x[0], pg1->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
//     return 1;
//   } else {
//     return 0;
//   }
// }
//
// int check4containment(polygon_t *pg, double x, double y) {
//   int counter1 = 0;
//   int counter2 = 0;
//
//   for (int i = 0; i < pg-> npoints; i++) {
//     int i2 = (i + 1) % pg->npoints;
//     double cp = (pg->x[i2] - pg->x[i] * (y - pg->y[i])) - (x - pg->x[i] * (pg->y[i2] - pg->y[i]));
//     if (cp > 0) {
//       counter1++;
//     } else if (cp < 0) {
//       counter2++;
//     }
//   }
//
//   if (counter1 == 0) {
//     return 1;
//   } else if (counter2 == 0) {
//     return 1;
//   } else {
//     return 0;
//   }
// }

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

void rotate2(pg_vector_t *rect_vec, pg_vector_t *transformed_vec, double angle, double x_pivot, double y_pivot) {
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
