#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M_PI 3.14159265358979323846

typedef struct polygon {
  int npoints;
  double x[16];
  double y[16];
} polygon_t;

// polygon_t poly(File *f) {
//   polygon_t pg = {0};
//   double x = 0;
//   double y = 0;
//   double rot = 0;
//   //return x y rot npoints values from fscanf
//   //check for npoints < 16 and error if not
//   //for i < npoints, read file and set return values to x, repeat with y
//   rot *= M_PI / 180;
//
//   for (int i = 0; i < npoints; i++) {
//     double transx = x + cos(rot) * pg.x[i] - sin(rot) * pg.y[i];
//     double transy = y + sin(rot) * pg.x[i] + cos(rot) * pg.y[i];
//     pg.x[i] = transx;
//     pg.y[i] = transy;
//   }
//
//   return pg;
// } //store values in pg

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
} //check if 2 lines (2 endpoints each) collide

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
} //check for 2 polygon intersection

int pg_collision(polygon_t *pg1, polygon_t *pg2) {
  if (pg_intersection(pg1,pg2) == 1) {
    return 1;
  } else if (check4containment(pg1, pg1->x[0], pg1->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
    return 1;
  } else if (check4containment(pg2, pg2->x[0], pg2->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
    return 1;
  } else {
    return 0;
  }
} // check for 2 polygon collision

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
} //check if polygon contains a point (x,y) of the other polygon

// //Open file using fopen, skip first 256 characters using fgets and set pointer to start of new line
// //Error messages setup
// polygon_t pg1 = poly(f); //datapoints
// polygon_t pg2 = poly(f);//datapoints
// //check collision
// if(pg_collision(&pg1, &pg2)) {
//   printf("collision!");
// } else {
//   printf("no collision\n");
// }
// return 0;
