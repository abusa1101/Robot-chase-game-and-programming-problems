#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#define M_PI 3.14159265358979323846

typedef struct polygon {
  int npoints;
  double x[16];
  double y[16];
} polygon_t;

polygon_t poly_scan(FILE *f) {
  polygon_t pg = {0};
  double x = 0;
  double y = 0;
  double rot = 0;

  int value = fscanf(f, "%lf %lf %lf %d", &x, &y, &rot, &pg.npoints);
  if (value != 4) {
      fprintf(stderr, "Error: Need FOUR entries\n");
      exit(1);
  }

  rot *= M_PI / 180;

  if (pg.npoints > 16) {
      fprintf(stderr, "Number of points greater than 16\n");
      exit(1);
  }

  for (int i = 0; i < pg.npoints; i++) { //store x values for n points
      value = fscanf(f, "%lf", &pg.x[i]);
      if (value != 1) {
          fprintf(stderr, "Cannot read x");
          exit(1);
      }
  }
  for (int i = 0; i < pg.npoints; i++) { //store y values for n points
      value = fscanf(f, "%lf", &pg.y[i]);
      if (value != 1) {
          fprintf(stderr, "Cannot read x");
          exit(1);
      }
  }

  for (int i = 0; i < pg.npoints; i++) {
    double trans_x = x + cos(rot) * pg.x[i] - sin(rot) * pg.y[i];
    double trans_y = y + sin(rot) * pg.x[i] + cos(rot) * pg.y[i];
    pg.x[i] = trans_x;
    pg.y[i] = trans_y;
  }

  return pg;
} //store values in pg

_Bool line_intersection(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3) {
  double cp0 = ((x3 - x2) * (y0 - y2)) - ((x0 - x2) * (y3 - y2)); //cp for p0
  double cp1 = ((x3 - x2) * (y1 - y2)) - ((x1 - x2) * (y3 - y2)); //cp for p1
  double check_cp01 = cp0 * cp1;

  double cp2 = ((x1 - x0) * (y2 - y0)) - ((x2 - x0) * (y1 - y0)); //cp for p2
  double cp3 = ((x1 - x0) * (y3 - y0)) - ((x3 - x0) * (y1 - y0)); //cp for p3
  double check_cp23 = cp2 * cp3;

  // if(check_cp01 <= 0 && check_cp23 <= 0) {
  //   if ((check_cp01 != 0 || check_cp23 != 0)) {
  //     return 1;
  //   }
  // } else {
  //   return 0;
  // }
  return check_cp01 <= 0 && check_cp23 <= 0 && (check_cp01 != 0 || check_cp23 != 0);
} //check if 2 lines (2 endpoints each) collide

_Bool pg_intersection(polygon_t *pg1, polygon_t *pg2) {
  for (int i = 0; i < pg1->npoints; i++) {
    int i2 = (i + 1) % pg1->npoints; //check hw2 document for this indexing reference
    double x0 = pg1->x[i];
    double y0 = pg1->y[i];
    double x1 = pg1->x[i2];
    double y1 = pg1->y[i2];
    for (int j = 0; j < pg2->npoints; j++) {
      int j2 = (j + 1) % pg2-> npoints;
      double x2 = pg2->x[j];
      double y2 = pg2->y[j];
      double x3 = pg2->x[j2];
      double y3 = pg2->y[j2];
      if (line_intersection(x0, y0, x1, y1, x2, y2, x3, y3)) {
        return true;
      }
    }
  }
  return false;
} //check for 2 polygon intersection

_Bool check4containment(polygon_t *pg, double x, double y) {
  int counter1 = 0;
  int counter2 = 0;
  for (int i = 0; i < pg-> npoints; i++) {
    int i2 = (i + 1) % pg->npoints;
    double cp = ((y - pg->y[i]) * (pg->x[i2] - pg->x[i])) - ((x - pg->x[i]) * (pg->y[i2] - pg->y[i]));
    if (cp > 0) {
      counter1++;
    } else if (cp < 0) {
      counter2++;
    }
  }
  // if (counter1 == 0) {
  //   return 1;
  // } else if (counter2 == 0) {
  //   return 1;
  // } else {
  //   return 0;
  // }
  return counter1 == 0 || counter2 == 0;
}

_Bool pg_collision(polygon_t *pg1, polygon_t *pg2) {
  // if (pg_intersection(pg1,pg2) == 1) {
  //   return 1;
  // } else if (check4containment(pg2, pg1->x[0], pg1->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
  //   return 1;
  // } else if (check4containment(pg1, pg2->x[0], pg2->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
  //   return 1;
  // } else {
  //   return 0;
  // }
  return pg_intersection(pg1,pg2) || check4containment(pg2, pg1->x[0], pg1->y[0]) ||
         check4containment(pg1, pg2->x[0], pg2->y[0]);
} // check for 2 polygon collision

int main(void) {
    FILE *f = fopen("polygons.csv", "r");
    if (!f) {
        fprintf(stderr, "Error: Missing file.\n");
        return 1;
    }
    char text[256];
    fgets(text, sizeof(text), f);
    polygon_t pg1 = poly_scan(f);
    polygon_t pg2 = poly_scan(f);
    fclose(f);
    if(pg_collision(&pg1, &pg2)) {
      printf("collision!");
    } else {
      printf("no collision\n");
    }
    return 0;
}
