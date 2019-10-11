// //activateMove(&game);
//  check4collision(robot, lamp) {
//      if(pg_collision(&rob, &lamp)) {
//        printf("collision!");
//      } else {
//        printf("no collision\n");
//      }
//  }
//  void retreat(game_t *game) {
//      int flag = check4collision(game);
//      while (flag == 1) {
//          game->rpos.x += 0.5; //or y?
//          game->rpos.y += 0.5;
//          flag = check4collision(game);
//      }
//  }
//gx_update(&bmp, &game, color_back, color_lamp, color_robot);

// double h = 12;
// double w = 12;
// double xcl1 = 124.1;
// double ycl1 = 224.1;
// double p1xl1 = xcl1 - h / 2.0;
// double p1yl1 = ycl1 - w / 2.0;
// double p2xl1 = xcl1 - h / 2.0;
// double p2yl1 = ycl1 + w / 2.0;
// double p3xl1 = xcl1 + h / 2.0;
// double p3yl1 = ycl1;
// lamp1->c_x = xcl1;
// lamp1->c_y = ycl1;
// lamp1->x[0] = p3xl1;
// lamp1->y[0] = p3yl1;
// lamp1->x[1] = p2xl1;
// lamp1->y[1] = p2yl1;
// lamp1->x[2] = p1xl1;
// lamp1->y[2] = p1yl1;
//
// double xcl2 = 349.1;
// double ycl2 = 99.1;
// double p1xl2 = xcl2 - h / 2.0;
// double p1yl2 = ycl2 - w / 2.0;
// double p2xl2 = xcl2 - h / 2.0;
// double p2yl2 = ycl2 + w / 2.0;
// double p3xl2 = xcl2 + h / 2.0;
// double p3yl2 = ycl2;
// lamp2->c_x = xcl2;
// lamp2->c_y = ycl2;
// lamp2->x[0] = p3xl2;
// lamp2->y[0] = p3yl2;
// lamp2->x[1] = p2xl2;
// lamp2->y[1] = p2yl2;
// lamp2->x[2] = p1xl2;
// lamp2->y[2] = p1yl2;
//
// double xcl3 = 449.1;
// double ycl3 = 349.1;
// double p1xl3 = xcl3 - h / 2.0;
// double p1yl3 = ycl3 - w / 2.0;
// double p2xl3 = xcl3 - h / 2.0;
// double p2yl3 = ycl3 + w / 2.0;
// double p3xl3 = xcl3 + h / 2.0;
// double p3yl3 = ycl3;
// lamp3->c_x = xcl3;
// lamp3->c_y = ycl3;
// lamp3->x[0] = p3xl3;
// lamp3->y[0] = p3yl3;
// lamp3->x[1] = p2xl3;
// lamp3->y[1] = p2yl3;
// lamp3->x[2] = p1xl3;
// lamp3->y[2] = p1yl3;

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
}

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
}

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
}

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
