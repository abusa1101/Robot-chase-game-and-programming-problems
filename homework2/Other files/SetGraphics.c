pg_vector_t rect_vec = {0};
pg_create(&rect_vec);
pg_vector_t rob_vec = {0};
pg_create(&rob_vec);
pg_vector_t trans1_vec = {0};
pg_create(&trans1_vec);
pg_vector_t trans2_vec = {0};
pg_create(&trans2_vec);
pg_vector_t trans3_vec = {0};
pg_create(&trans3_vec);
pg_vector_t fillbuff = {0};
pg_create2(&fillbuff);
pg_vector_t l1_vec = {0};
pg_create(&l1_vec);
pg_vector_t l2_vec = {0};
pg_create(&l2_vec);
pg_vector_t l3_vec = {0};
pg_create(&l3_vec);
pg_vector_t fillbuff1 = {0};
pg_create(&fillbuff1);
pg_vector_t fillbuff2 = {0};
pg_create(&fillbuff2);
pg_vector_t fillbuff3 = {0};
pg_create(&fillbuff3);

//border
double xcb = 320.0;
double ycb = 240.0;
give_rect(&rect_vec, 600.0, 440.0, xcb, ycb);
cd2pixel(&rect_vec);
pg_draw(&bmp, color, &rect_vec, 0);

//robot
give_tri(&rob_vec, 21.0, 28.0, 320, 240);
cd2pixel(&rob_vec);
tri_draw(&bmp, color, &rob_vec);
callb2(&fillbuff, &rob_vec);
tri_fill(&bmp, color_robot, &rob_vec, &fillbuff);

//lamps
double lamp_size = 12.0;
double xcl1 = 124.1;
double ycl1 = 224.1;
give_rect(&l1_vec, lamp_size, lamp_size, xcl1, ycl1);
rotate2(&l1_vec, &trans1_vec, 45, xcl1, ycl1);
cd2pixel(&trans1_vec);
pg_draw(&bmp, color_lamp, &trans1_vec, 0);
callb2(&fillbuff1, &trans1_vec);
tri_fill(&bmp, color_lamp, &trans1_vec, &fillbuff1);

//int n = fillbuff1.size;
//printf("%d ", n);
//  for (int i = 0; i <fillbuff->size; i++) {
//   printf("%f ", fillbuff->data[i].x);
//   printf("%f\n", fillbuff->data[i].y);
//   //printf("%d ", i);
// }

double xcl2 = 349.1;
double ycl2 = 99.1;
give_rect(&l2_vec, lamp_size, lamp_size, xcl2, ycl2);
rotate2(&l2_vec, &trans2_vec, 45, xcl2, ycl2);
cd2pixel(&trans2_vec);
pg_draw(&bmp, color_lamp, &trans2_vec, 0);
callb2(&fillbuff2, &trans2_vec);
tri_fill(&bmp, color_lamp, &trans2_vec, &fillbuff2);
//int n2 = fillbuff2.size;
//printf("%d ", n2);

double xcl3 = 449.1;
double ycl3 = 349.1;
give_rect(&l3_vec, lamp_size, lamp_size, xcl3, ycl3);
rotate2(&l3_vec, &trans3_vec, 45, xcl3, ycl3);
cd2pixel(&trans3_vec);
pg_draw(&bmp, color_lamp, &trans3_vec, 0);
callb2(&fillbuff3, &trans3_vec);
tri_fill(&bmp, color_lamp, &trans3_vec, &fillbuff3);
//int n3 = fillbuff3.size;
//printf("%d ", n3);

pg_free(&fillbuff);
pg_free(&fillbuff1);
pg_free(&fillbuff2);
pg_free(&fillbuff3);
pg_free(&rob_vec);
pg_free(&rect_vec);
pg_free(&l1_vec);
pg_free(&l2_vec);
pg_free(&l3_vec);
pg_free(&trans1_vec);
pg_free(&trans2_vec);
pg_free(&trans3_vec);



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



/////////////////


//#ifndef MOVEMENT_H
//#define MOVEMENT_H
// typedef struct pos_data {
//     double x;
//     double y;
// } pos_data_t;
//
// typedef struct rob_position {
//     pos_data_t *data;
//     int capacity;
//     int size;
// } rob_position_t;
//
// typedef struct unitvec {
//     double x;
//     double y;
// } unitvec_t;
//
// typedef struct lamp {
//     rob_position_t location;
//     color_bgr_t color;
//     double size;
//     double rot;
//     double power;
// } lamp_t;
//
// typedef struct compare {
//     double l1;
//     double l2;
//     double l3;
// } compare_t;
//
// typedef struct robot {
//   rob_position_t location;
//   color_bgr_t color;
//   unitvec_t dir;
//   unitvec_t eye_l;
//   unitvec_t eye_r;
//   double dist_sq;
//   double move_l;
//   double move_r;
//   double theta;
//   double fwd_dist;
//   double max_movement;
//   double wheel_base;
//   double width;
//   double length;
// } robot_t;
//
// typedef struct game {
//   color_bgr_t color;
//   robot_t robot;
//   lamp_t lamp;
//   double n_lamp;
// } game_t;

////////////////////////////////////////////FUNCTIONS/////////////////////////////////////////////////////////////
//Movement
//void setConstants(robot_t robot, lamp_t lamp1, lamp_t lamp2, lamp_t lamp3);
// // double dotP(game_t game);
// // void rotateP(double dir, double angle, double x_pivot, double y_pivot);
// // void activateMove(game_t game);
//
//Graphics
 //void initGraphics(game_t game, pg_vector_t *rect_vec);
// // void resetGraphics (game_t game, pg_points_t *rect_vec);
// // void updateGraphics(game_t game, pg_points_t *rect_vec);
//
// //Collision
// // int line_intersection(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);
// // int pg_intersection(polygon_t *pg1, polygon_t *pg2);
// // int pg_collision(polygon_t *pg1, polygon_t *pg2);
// // int check4containment(polygon_t *pg, double x, double y);
//


//#endif
