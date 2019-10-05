//   for (int y = tri_vec->data[1].y; y <tri_vec->data[0].y; y++) {
//     for (int x = tri_vec->data[1].x; x <tri_vec->data[2].x; x++) {
//       bmp->data[y * bmp->width + x] = color;
//     }
// }

// int x0[bmp->height];
// //int x1[bmp->height];
// int val = tri_vec->data[3].y;
// for (int i = 0; i <21; i++) {
//   x0[i] = val;
//   // x1[i] = 28;
//   printf("%d ", x0[i]);
//   for (int y = tri_vec->data[3].y; y <tri_vec->data[1].y; y++) {
//     bmp->data[y * bmp->width + x0[i]] = color;
//   }
//   val++;
// }
for each line in polygon, represented by index i that the line starts on
for (int i = 0; i <bmp->height - 1; i++) {
for (int i = 0; )
  i2 = (i + 1) % n; // index of second point of the line
  points = rasterize_line(point at i, point at i2);
  for (int i = 0; i <bmp->height - 1; i++) {
    x0[i] = -1;
    x1[i] = -1;
  }
  for point (x, y) in points {
    // follow logic in homework2 document to modify x0[y] and x1[y]
    for (int y = 0; y <bmp->height - 1; y++) {
      if (x0[y] == -1) {
        x0[y] = x;
        x1[y] = x;
      } else {
        x0[y] = min(x0[y], x);
        x1[y] = max(x1[y], x);
      }
    }
  }
}

void test2 (bitmap_t *bmp, color_bgr_t color) {
  bresenham(10,10,200,200, &bmp, color);
}

void test3 (bitmap_t *bmp, color_bgr_t color) {
  double xc = -1.0;
  double yc = 0.0;
  pg_vector_t rect_vec = {0};
  pg_create(&rect_vec);
  give_rect(&rect_vec, 4.0, 4.0, xc, yc);
  pg_draw(&bmp, color, &rect_vec);
}

void test4 (bitmap_t *bmp, color_bgr_t color) {
  double xc = 1.0;
  double yc = 2.0;
  pg_vector_t rect_vec = {0};
  pg_create(&rect_vec);
  give_rect(&rect_vec, 4.0, 4.0, xc, yc);
  cd2pixel(&rect_vec);
  pg_draw(&bmp, color, &rect_vec);
}

void test5 (bitmap_t *bmp, color_bgr_t color) {
  double xc = 1.0;
  double yc = 2.0;
  pg_vector_t rect_vec = {0};
  pg_create(&rect_vec);
  give_rect(&rect_vec, 5.0, 5.0, xc, yc);
  cd2pixel(&rect_vec);
  pg_draw(&bmp, color, &rect_vec);
}

void test6 (bitmap_t *bmp, color_bgr_t color) {
  double xc = 320.0;
  double yc = 240.0;
  pg_vector_t rect_vec = {0};
  pg_create(&rect_vec);
  give_rect(&rect_vec, 600.0, 440.0, xc, yc);
  cd2pixel(&rect_vec);
  pg_draw(&bmp, color, &rect_vec);
}

void test7 (bitmap_t *bmp, color_bgr_t color) {
  double xc = 320.0;
  double yc = 240.0;
  pg_vector_t rect_vec = {0};
  pg_create(&rect_vec);
  give_rect(&rect_vec, 600.0, 440.0, xc, yc);
  cd2pixel(&rect_vec);
  pg_draw(&bmp, color, &rect_vec);
  pg_fill(&bmp, color, &rect_vec);
}

void test8 (bitmap_t *bmp, color_bgr_t color) {
}

void test9 (bitmap_t *bmp, color_bgr_t color) {
}


else if (input == 8) {
//   test8(&bmp, color);
// } else if (input == 9) {
//   test9(&bmp, color);
// } else {
// }


// bresenham(-1,0,4,4, &bmp, color);
// double xc = 320.0;
// double yc = 240.0;
// pg_vector_t rect_vec = {0};
// pg_create(&rect_vec);
// give_rect(&rect_vec, 600.0, 440.0, xc, yc);
// // for (int i = 0; i <rect_vec.size; i++) {
// //   printf("%f ", rect_vec.data[i].x);
// //   printf("%f\n", rect_vec.data[i].y);
// //   //bmp.data[rect_vec.data[i].y * bmp.width + rect_vec.data[i].x] = color;
// // }
// // printf("\n");
// cd2pixel(&rect_vec);
// // for (int i = 0; i <rect_vec.size; i++) {
// //   printf("%f ", rect_vec.data[i].x);
// //   printf("%f\n", rect_vec.data[i].y);
// //   //bmp.data[rect_vec.data[i].y * bmp.width + rect_vec.data[i].x] = color;
// // }
//
// // pg_vector_t transformed_vec = {0};
// // pg_create(&transformed_vec);
// // double xglobal = 2.0;
// // double yglobal = 2.0;
// // translate(&rect_vec, &transformed_vec, xglobal, yglobal);
//
// pg_draw(&bmp, color, &rect_vec);
// //pg_draw(&bmp, color, &transformed_vec);
// pg_fill(&bmp, color, &rect_vec);
//
// pg_vector_t tri_vec = {0};
// pg_create(&tri_vec);
//
// give_tri(&tri_vec, 21.0, 28.0, 400, 400);
// cd2pixel(&tri_vec);
// tri_draw(&bmp, color, &tri_vec);
// tri_fill(&bmp, color, &tri_vec);


if (p1x >= 0 && p1y >= 0) {
  pg_append(rect_vec,p1x, p1y);
} else {
  p1x = 0;
  p1y = 0;
  pg_append(rect_vec,p1x, p1y);
}
double p2x = xc + width / 2.0;
double p2y = yc - height / 2.0;
if (p2x >= 0 && p2y >= 0) {
  pg_append(rect_vec,p2x, p2y);
} else {
  p2x = 0;
  p2y = 0;
  pg_append(rect_vec,p2x, p2y);
}
double p3x = xc - width / 2.0;
double p3y = yc - height / 2.0;
if (p3x >= 0 && p3y >= 0) {
  pg_append(rect_vec,p3x, p3y);
} else {
  p3x = 0;
  p3y = 0;
  pg_append(rect_vec,p3x, p3y);
}
double p4x = xc - width / 2.0;
double p4y = yc + height / 2.0;
if (p4x >= 0 && p4y >= 0) {
  pg_append(rect_vec,p4x, p4y);
} else {
  p4x = 0;
  p4y = 0;
  pg_append(rect_vec,p4x, p4y);
}


void pg_draw(bitmap_t *bmp, color_bgr_t color, pg_vector_t *rect_vec) {
  //pg_vector_t *pixelpts = cd2pixel(points);
  // for(int i = 0; i < pixelpts->size; i++){
  //   bresenham(pixelpts->data[i].x, pixelpts->data[i].y, pixelpts->data[i+1].x, pixelpts->data[i+1].y, bmp, color);
  // }
  // bresenham(pixelpts->data[pixelpts->size - 1].x, pixelpts->data[pixelpts->size - 1].y, pixelpts->data[0].x, pixelpts->data[0].y, bmp, color);
  // pg_free(pixelpts);
  bresenham(rect_vec->data[0].x, rect_vec->data[0].y, rect_vec->data[1].x, rect_vec->data[1].y, bmp, color);
  bresenham(rect_vec->data[1].x, rect_vec->data[1].y, rect_vec->data[2].x, rect_vec->data[2].y, bmp, color);
  bresenham(rect_vec->data[2].x, rect_vec->data[2].y, rect_vec->data[3].x, rect_vec->data[3].y, bmp, color);
  printf("%d ", rect_vec->data[1].x);
  printf("%d ", rect_vec->data[1].y);
  printf("%d ", rect_vec->data[2].x);
  printf("%d ", rect_vec->data[2].y);
  bresenham(rect_vec->data[0].x, rect_vec->data[0].y, rect_vec->data[1].x, rect_vec->data[1].y, bmp, color);
  bresenham(rect_vec->data[1].x, rect_vec->data[1].y, rect_vec->data[2].x, rect_vec->data[2].y, bmp, color);
  bresenham(rect_vec->data[2].x, rect_vec->data[2].y, rect_vec->data[3].x, rect_vec->data[3].y, bmp, color);

double cd2pixel(double value) {
  pg_vector_t pixels;
  pg_create(&pixels);
  double epsilon = 1e-6;
  double tempx = points->data[0].x; //find a better way to do this perhaps??
  double tempy = points->data[0].y;
  for(int i = 0; i <points->size; i++) { //find min value and store in tempx,tempy
    if (tempx > points->data[i].x) {
      tempx = points->data[i].x;
    }
    if (tempy > points->data[i].y) {
      tempy = points->data[i].y;
    }
  }
  for(int i = 0; i <points->size; i++) { //check for min value and round off
    if (points->data[i].x == tempx) {
      pixels.data[i].x = ceil(points->data[i].x);
    } else {
      pixels.data[i].x = floor(points->data[i].x - epsilon);
    }
    if (points->data[i].y == tempy) {
      pixels.data[i].y = ceil(points->data[i].y);
    } else {
      pixels.data[i].y = floor(points->data[i].y - epsilon);
    }
  }
  pg_free(points);
  return &pixels;
} //MUST FREE THIS AS WELL!!!
