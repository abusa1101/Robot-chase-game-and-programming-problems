//TESTCASES
// In this first task, and the first test case for this problem, we need to
// construct a valid Microsoft bitmap image.
DONE
> Comment out full rasterize polygon code basically;

// The second test case for this problem is to draw a white line from (10, 10) to
// (200, 200) on a black background of size 640 by 480
DONE
>Comment out all except this:
>>bresenham(10,10,200,200, &bmp, color);

// The third test case for this problem is to draw a 4 by 4 pixel white square
// centered at (0, 0).
NOT DONE?

> 0,0 is not exactly at top left. its at -1 0
>>double xc = -1.0;
double yc = 0.0;
pg_vector_t rect_vec = {0};
pg_create(&rect_vec);
give_rect(&rect_vec, 4.0, 4.0, xc, yc);
pg_draw(&bmp, color, &rect_vec);

// The fourth test case is to draw a 4 by 4 pixel white square centered at (2, 2).
// Now the whole square should be visible.
double xc = 1.0;
double yc = 2.0;
pg_vector_t rect_vec = {0};
pg_create(&rect_vec);
give_rect(&rect_vec, 4.0, 4.0, xc, yc);
cd2pixel(&rect_vec);
pg_draw(&bmp, color, &rect_vec);

// The fifth test case is to draw a 5 by 5 pixel white square centered at (2, 2).
// The whole square should also be visible. If you have trouble with this test,
// look back at how to do the polygon rounding again.
double xc = 1.0;
double yc = 2.0;
pg_vector_t rect_vec = {0};
pg_create(&rect_vec);
give_rect(&rect_vec, 5.0, 5.0, xc, yc);
cd2pixel(&rect_vec);
pg_draw(&bmp, color, &rect_vec);

// The sixth test case is to draw a 600 (width – 40) by 440 (height – 40) white
// border centered at the center of the image (640 / 2, 480 / 2). The border’s top
// left pixel will be at (20, 20) and the bottom right corner at (619, 459).

double xc = 320.0;
double yc = 240.0;
pg_vector_t rect_vec = {0};
pg_create(&rect_vec);
give_rect(&rect_vec, 600.0, 440.0, xc, yc);
cd2pixel(&rect_vec);
pg_draw(&bmp, color, &rect_vec);

// The seventh test case is to fill in the rectangle in the sixth test case.
// Filled in pixels should still reach exactly from (20, 20) to (619, 459).
double xc = 320.0;
double yc = 240.0;
pg_vector_t rect_vec = {0};
pg_create(&rect_vec);
give_rect(&rect_vec, 600.0, 440.0, xc, yc);
cd2pixel(&rect_vec);
pg_draw(&bmp, color, &rect_vec);
pg_fill(&bmp, color, &rect_vec);

// The eighth test case is to fill in a robot shape, translated to/centered at
// (400, 400). It should end up 21 pixels wide on the y-axis and then 28 long on
// the x-axis. It will not appear completely symmetrical.


// The ninth test case is the same as the seventh case, but with the robot rotated
// 30 degrees counterclockwise. Note it should still be centered at (400, 400).
