#include "bmp.h"
#include "chase_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//Collision & Polygons
int pg_collision(vector_xy_t *pg1, vector_xy_t *pg2) {
    if ((pg_intersection(pg1, pg2) == 1) ||
        (check4containment(pg2, pg1->data[0].x, pg1->data[0].y) == 1) ||
        (check4containment(pg1, pg2->data[0].x, pg2->data[0].y) == 1)) {
        return 1;
    }
    return 0;
}

bool pg_intersection(vector_xy_t *pg1, vector_xy_t *pg2) {
    for (int i = 0; i < pg1->size; i++) {
        int i2 = (i + 1) % pg1->size;
        double x0 = pg1->data[i].x;
        double y0 = pg1->data[i].y;
        double x1 = pg1->data[i2].x;
        double y1 = pg1->data[i2].y;
        for (int j = 0; j < pg2->size; j++) {
            int j2 = (j + 1) % pg2->size;
            double x2 = pg2->data[j].x;
            double y2 = pg2->data[j].y;
            double x3 = pg2->data[j2].x;
            double y3 = pg2->data[j2].y;
            if (line_intersection(x0, y0, x1, y1, x2, y2, x3, y3)) {
                return true;
            }
        }
    }
    return false;
}

bool line_intersection(double x0, double y0, double x1, double y1,
                       double x2, double y2, double x3, double y3) {
    double cp0 = ((x3 - x2) * (y0 - y2)) - ((x0 - x2) * (y3 - y2)); //cp for p0
    double cp1 = ((x3 - x2) * (y1 - y2)) - ((x1 - x2) * (y3 - y2)); //cp for p1
    double check_cp01 = cp0 * cp1;

    double cp2 = ((x1 - x0) * (y2 - y0)) - ((x2 - x0) * (y1 - y0)); //cp for p2
    double cp3 = ((x1 - x0) * (y3 - y0)) - ((x3 - x0) * (y1 - y0)); //cp for p3
    double check_cp23 = cp2 * cp3;

    return check_cp01 <= 0 && check_cp23 <= 0 && (check_cp01 != 0 || check_cp23 != 0);
}

bool check4containment(vector_xy_t *pg, double x, double y) {
    int counter1 = 0;
    int counter2 = 0;
    for (int i = 0; i < pg->size; i++) {
        int i2 = (i + 1) % pg->size;
        double cp = ((y - pg->data[i].y) * (pg->data[i2].x - pg->data[i].x)) -
                    ((x - pg->data[i].x) * (pg->data[i2].y - pg->data[i].y));
        if (cp > 0) {
            counter1++;
        } else if (cp < 0) {
            counter2++;
        }
    }
    return counter1 == 0 || counter2 == 0;
}

bool tile_collision(robot_t *robot, double tile_x, double tile_y) {
    vector_xy_t *robot_vec = robot2(robot);
    vector_xy_t *tile_vec = gx_rect(BLOCK_SIZE, BLOCK_SIZE);
    gx_trans(tile_x * BLOCK_SIZE, tile_y * BLOCK_SIZE, tile_vec);
    bool collides = pg_collision(robot_vec, tile_vec);
    vector_free(tile_vec);
    vector_free(robot_vec);
    return collides;
}

bool resolve_tile_collision(robot_t *robot) {
    int map_x = (robot->x * MAP_W / WIDTH);
    int map_y = (robot->y * MAP_H / HEIGHT);
    bool in_collision = true;
    bool had_collision = false;
    while (in_collision) {
        in_collision = false;
        for (int y = (int)max(map_y - 1, 0); y <= map_y + 1; y++) {
            for (int x = (int)max(map_x - 1, 0); x <= map_x + 1; x++) {
                if (MAP[y * MAP_W + x] == 'X') {
                    if (tile_collision(robot, x, y)) {
                        had_collision = true;
                        in_collision = true;
                        double dx = (x + 0.5) * BLOCK_SIZE - robot->x;
                        double dy = (y + 0.5) * BLOCK_SIZE - robot->y;
                        double dist = sqrt(pow(dx, 2) + pow(dy, 2));
                        double dir_x = dx / dist;
                        double dir_y = dy / dist;
                        robot->x -= 0.5 * dir_x;
                        robot->y -= 0.5 * dir_y;
                    }
                }
            }
        }
    }
    return had_collision;
}
