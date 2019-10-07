#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "bmp.h"
#define M_PI 3.14159265358979323846

double dotP(double dir[], double eye[]) {
  double product = 0;
  for (int i = 0; i < 2; i++) { //2 because 2D ai + bj
    product = product + dir[i] * eye[i];
  }
  return product;
}

void rotateP(double dir, double angle, double x_pivot, double y_pivot) {
  double rotx = 0;
  double roty = 0;
  // int i = 0;
  // while (i < 2) {
  double x_shifted = dir[i] - x_pivot;
  double y_shifted = dir[i] - y_pivot;
  rotx = x_pivot + (x_shifted * COS(angle) - y_shifted * SIN(angle));
  roty = y_pivot + (x_shifted * SIN(angle) + y_shifted * COS(angle));
  //    pg_append(transformed_vec, rotx, roty);
  //     i++;
  // }
}

void activateMove(game_t game) {
  for (int i = 0; i < game.n_lamps; i++) { //Lamp1
    game.robot.dist_sq = pow(game.lamp.location.x - game.robot.location.x, 2) + pow(game.lamp.location.y - game.robot.location.y, 2);
    game.robot.dir.x = (game.lamp.location.x - game.robot.location.x)/ sqrt(game.robot.dist_sq);
    game.robot.dir.y = (game.lamp.location.y - game.robot.location.y)/ sqrt(game.robot.dist_sq);
    game.robot.eye_l.x = (game.lamp.location.x - game.robot.location.x) / cos((game.robot.theta + 60)* M_PI / 180);
    game.robot.eye_l.y = (game.lamp.location.y - game.robot.location.y) / cos((game.robot.theta + 60)* M_PI / 180);
    game.robot.eye_r.x  = (game.lamp.location.x - game.robot.location.x) / cos((game.robot.theta - 60)* M_PI / 180);
    game.robot.eye_r.y  = (game.lamp.location.x - game.robot.location.x) / cos((game.robot.theta - 60)* M_PI / 180);
    game.robot.move_l += max(0.0, dotP(game.robot.dir, game.robot.eye_l) * game.lamp.power / (game.robot.dist_sq);
    game.robot.move_r += max(0.0, dotP(game.robot.dir, game.robot.eye_r) * game.lamp.power / (game.robot.dist_sq);

    game.robot.move_l = min(game.robot.max_movement, game.robot.move_l);
    game.robot.move_r = min(game.robot.max_movement, game.robot.move_r);
    game.robot.theta += (game.robot.move_r - game.robot.move_l) / (game.robot.wheel_base);
    game.robot.fwd_dist = (game.robot.move_r + game.robot.move_l) / 2;
  }

}

// double maxof3(double v1, double v2, double v3) {
//     if (v1 >= v2 && v1 >= v3) {
//       double maxval = v1;
//     } else if (v2 >= v1 && v2 >= v3) {
//       double maxval = v2;
//     } else if (v3 >= v1 && v3 >= v2) {
//       double maxval = v3;
//     }
//     return maxval;
// }

int line_intersection(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3) {
  double cp0 = (x3 - x2) * (y0 - y2) - (x0 - x2) * (y3 - y2); //cp for p0
  double cp1 = (x3 - x2) * (y1 - y2) - (x1 - x2) * (y3 - y2); //cp for p1
  double cp2 = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0); //cp for p2
  double cp3 = (x1 - x0) * (y3 - y0) - (x3 - x0) - (y1 - y0); //cp for p3

  double check_cp23 = cp2 * cp3;
  double check_cp01 = cp0 * cp1;

  if(check_cp01 <= 0 && check_cp23 <= 0) {
    if ((check_cp01 != 0 || check_cp23 != 0)) {
      return 1;
    }
  } else {
    return 0;
  }
}

int pg_intersection(polygon_t *pg1, polygon_t *pg2) {
  for (int i = 0; i < pg1->npoints; i++) {
    int i2 = (i + 1) % pg->npoints; //check hw2 document for this indexing reference
    double x0 = pg1->x[i];
    double y0 = pg1->y[i];
    double x1 = pg1->x[i2];
    double y1 = pg1->y[i2];
    for (int j = 0; j < pg2->npoints; j++) {
      int j2 = (j + 1) % pg2-> npoints;
      double x2 = pg2->x[j];
      double y2 = pg2->x[j];
      double x3 = pg2->x[j2];
      double y3 = pg2->x[j2];
      if (line_intersection(x0, y0, x1, y1, x2, y2, x3, y3) == 1) {
        return 1;
      } else {
        return 0;
      }
    }
  }
}

int pg_collision(polygon_t *pg1, polygon_t *pg2) {
  if (pg_intersection(pg1,pg2) == 1) {
    return 1;
  } else if (check4containment(pg1, pg2->x[0], pg2->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
    return 1;
  } else if (check4containment(pg2, pg1->x[0], pg1->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
    return 1;
  } else {
    return 0;
  }
}

int check4containment(polygon_t *pg, double x, double y) {
  int counter1 = 0;
  int counter2 = 0;

  for (int i = 0; i < pg-> npoints; i++) {
    int i2 = (i + 1) % pg->npoints;
    double cp = (pg->x[i2] - pg->x[i] * (y - pg->y[i])) - (x - pg->x[i] * (pg->y[i2] - pg->y[i]));
    if (cp > 0) {
      counter1++;
    } else if (cp < 0) {
      counter2++;
    }
  }

  if (counter1 == 0) {
    return 1;
  } else if (counter2 == 0) {
    return 1;
  } else {
    return 0;
  }
}
