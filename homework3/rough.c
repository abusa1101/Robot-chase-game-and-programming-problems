#include "bmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>


int main(void) {
    double rob_x = 0;
    double rob_y = 0;
    double x = rob_x - 60; //Tile 1 topleft x
    double y = rob_y + 60; //Tile 1 topleft y
    for (int i = 0; i < 9; i++) {
        //bool collision_status = resolve_tile_collision(robot, x, y);
        printf("i %d: %f, %f\n", i, x, y);
        x += 40;
        y += -40;
        if (i == 2) {
            x += -120;
            y += 120;
        } else if (i == 5) {
            x += -120;
            y += 120;
        }
    }
}
