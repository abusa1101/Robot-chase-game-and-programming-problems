#include "bmp.h"
#include "vectors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void CallFunctions(bitmap_t bmp, color_bgr_t color, int input,
                   pg_vector_t rect_vec, pg_vector_t transformed_vec, pg_vector_t fillbuff) {
    if (input == 2) {
        bresenham(10, 10, 200, 200, &bmp, color);
    } else if (input == 3) {
        double xc = 0.0;
        double yc = 0.0;
        give_rect(&rect_vec, 4.0, 4.0, xc, yc);
        cd2pixel(&rect_vec);
        pg_draw(&bmp, color, &rect_vec, 3);
    } else if (input == 4) {
        double xc = 2.0;
        double yc = 2.0;
        give_rect(&rect_vec, 4.0, 4.0, xc, yc);
        cd2pixel(&rect_vec);
        pg_draw(&bmp, color, &rect_vec, 0);
    } else if (input == 5) {
        double xc = 2.0;
        double yc = 2.0;
        give_rect(&rect_vec, 5.0, 5.0, xc, yc);
        cd2pixel(&rect_vec);
        pg_draw(&bmp, color, &rect_vec, 0);
    } else if (input == 6) {
        double xc = 320.0;
        double yc = 240.0;
        give_rect(&rect_vec, 600.0, 440.0, xc, yc);
        cd2pixel(&rect_vec);
        pg_draw(&bmp, color, &rect_vec, 0);
    } else if (input == 7) {
        double xc = 320.0;
        double yc = 240.0;
        give_rect(&rect_vec, 600.0, 440.0, xc, yc);
        cd2pixel(&rect_vec);
        pg_draw(&bmp, color, &rect_vec, 0);
        pg_fill(&bmp, color, &rect_vec);
    } else if (input == 8) {
        give_tri(&rect_vec, 21.0, 28.0, 400, 400);
        cd2pixel(&rect_vec);
        tri_draw(&bmp, color, &rect_vec);
        callb2(&fillbuff, &rect_vec);
        tri_fill(&bmp, color, &rect_vec, &fillbuff);
    } else if (input == 9) {
        give_tri(&rect_vec, 21.0, 28.0, 400, 400);
        //cd2pixel(&rect_vec);
        rotate(&rect_vec, &transformed_vec, -30);
        //cd2pixel(&transformed_vec);
        //tri_draw(&bmp, color, &transformed_vec);
        callb2(&fillbuff, &transformed_vec);
        cd2pixel(&fillbuff);
        tri_fill(&bmp, color, &transformed_vec, &fillbuff);
        // bmp.data[393 * bmp.width + 412].r = 255;
        // bmp.data[393 * bmp.width + 412].g = 255;
        // bmp.data[393 * bmp.width + 412].b = 255;
        //
        // bmp.data[393 * bmp.width + 408].r = 0;
        // bmp.data[393 * bmp.width + 408].g = 0;
        // bmp.data[393 * bmp.width + 408].b = 0;
        //
        // bmp.data[393 * bmp.width + 407].r = 0;
        // bmp.data[393 * bmp.width + 407].g = 0;
        // bmp.data[393 * bmp.width + 407].b = 0;
        //
        // bmp.data[393 * bmp.width + 406].r = 0;
        // bmp.data[393 * bmp.width + 406].g = 0;
        // bmp.data[393 * bmp.width + 406].b = 0;
    }
}
