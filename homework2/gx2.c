//activateMove(&game);
 check4collision(robot, lamp) {


     if(pg_collision(&rob, &lamp)) {
       printf("collision!");
     } else {
       printf("no collision\n");
     }
 }


 void retreat(game_t *game) {
     int flag = check4collision(game);
     while (flag == 1) {
         game->rpos.x += 0.5; //or y?
         game->rpos.y += 0.5;
         flag = check4collision(game);
     }
 }
//gx_update(&bmp, &game, color_back, color_lamp, color_robot);


//Collision
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

int pg_intersection(polygon_t *rob, polygon_t *lamp) {
  for (int i = 0; i < rob->npoints; i++) {
    double x0 = rob->x[i];
    double y0 = rob->y[i];
    double x1 = rob->x[i+1];
    double y1 = rob->y[i+1];
    for (int j = 0; j < lamp->npoints; j++) {
      double x2 = lamp->x[j];
      double y2 = lamp->x[j];
      double x3 = lamp->x[j+1];
      double y3 = lamp->x[j+1];
      if (line_intersection(x0, y0, x1, y1, x2, y2, x3, y3) == 1) {
        return 1;
      } else {
        return 0;
      }
    }
  }
} //check for 2 polygon intersection

int pg_collision(polygon_t *rob, polygon_t *lamp) {
  if (pg_intersection(rob,lamp) == 1) {
    return 1;
  } else if (check4containment(rob, rob->x[0], rob->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
    return 1;
  } else if (check4containment(lamp, lamp->x[0], lamp->y[0]) == 1) { //check only one point of that polygon b/c either every point is contained or none
    return 1;
  } else {
    return 0;
  }
} // check for 2 polygon collision

int check4containment(polygon_t *rob, double x, double y) {
  int counter1 = 0;
  int counter2 = 0;

  for (int i = 0; i < rob-> npoints; i++) {
    double cp = (rob->x[i+1] - rob->x[i] * (y - rob->y[i])) - (x - rob->x[i] * (rob->y[i+1] - rob->y[i]));
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


void pg_store(vector_xy_t *lamp1_points, vector_xy_t *lamp2_points, vector_xy_t *lamp3_points) {
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

    vector_append(rob_points, p4x, p4y);
    vector_append(rob_points, p3x, p3y);
    vector_append(rob_points, p2x, p2y);
    vector_append(rob_points, p1x, p1y);

    double xc = 0;
    double yc = 0;
    double p1x = xc - h / 2.0;
    double p1y = yc - w / 2.0;
    double p2x = xc - h / 2.0;
    double p2y = yc + w / 2.0;
    double p3x = xc + h / 2.0;
    double p3y = yc;

    vector_append(points, p3x, p3y);
    vector_append(points, p2x, p2y);
    vector_append(points, p1x, p1y);


}
