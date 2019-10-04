// #include "bmp.h"
// #include "test.h"
// #include "vectors.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <math.h>
//
// void test2 (bitmap_t *bmp, color_bgr_t color) {
//   bresenham(10,10,200,200, &bmp, color);
// }
//
// void test3 (bitmap_t *bmp, color_bgr_t color) {
//   double xc = -1.0;
//   double yc = 0.0;
//   pg_vector_t rect_vec = {0};
//   pg_create(&rect_vec);
//   give_rect(&rect_vec, 4.0, 4.0, xc, yc);
//   pg_draw(&bmp, color, &rect_vec);
// }
//
// void test4 (bitmap_t *bmp, color_bgr_t color) {
//   double xc = 1.0;
//   double yc = 2.0;
//   pg_vector_t rect_vec = {0};
//   pg_create(&rect_vec);
//   give_rect(&rect_vec, 4.0, 4.0, xc, yc);
//   cd2pixel(&rect_vec);
//   pg_draw(&bmp, color, &rect_vec);
// }
//
// void test5 (bitmap_t *bmp, color_bgr_t color) {
//   double xc = 1.0;
//   double yc = 2.0;
//   pg_vector_t rect_vec = {0};
//   pg_create(&rect_vec);
//   give_rect(&rect_vec, 5.0, 5.0, xc, yc);
//   cd2pixel(&rect_vec);
//   pg_draw(&bmp, color, &rect_vec);
// }
//
// void test6 (bitmap_t *bmp, color_bgr_t color) {
//   double xc = 320.0;
//   double yc = 240.0;
//   pg_vector_t rect_vec = {0};
//   pg_create(&rect_vec);
//   give_rect(&rect_vec, 600.0, 440.0, xc, yc);
//   cd2pixel(&rect_vec);
//   pg_draw(&bmp, color, &rect_vec);
// }
//
// void test7 (bitmap_t *bmp, color_bgr_t color) {
//   double xc = 320.0;
//   double yc = 240.0;
//   pg_vector_t rect_vec = {0};
//   pg_create(&rect_vec);
//   give_rect(&rect_vec, 600.0, 440.0, xc, yc);
//   cd2pixel(&rect_vec);
//   pg_draw(&bmp, color, &rect_vec);
//   pg_fill(&bmp, color, &rect_vec);
// }
//
// void test8 (bitmap_t *bmp, color_bgr_t color) {
// }
//
// void test9 (bitmap_t *bmp, color_bgr_t color) {
// }
