#include "bmp.h"
#include "chase_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#define M_PI 3.14159265358979323846
#define WIDTH 640
#define HEIGHT 480
#define BLOCK_SIZE 40
#define MAP "XXXXXXXXXXXXXXXX" \
            "X              X" \
            "X  XXXX   XXX  X" \
            "X   XX      X  X" \
            "X       XXX    X" \
            "XXXXXX         X" \
            "X         XXXXXX" \
            "X    XXX       X" \
            "X  XX     XX   X" \
            "X   X    XX    X" \
            "X      XXX     X" \
            "XXXXXXXXXXXXXXXX"
#define MAP_W (WIDTH / BLOCK_SIZE)
#define MAP_H (HEIGHT / BLOCK_SIZE)
#define MAX_VEL 12
#define ROB_W 20
#define ROB_L 26.67


// Vector Operations
vector_xy_t *vector_create(void) {
    vector_xy_t *v = malloc(sizeof(vector_xy_t));
    v->capacity = 8;
    v->size = 0;
    v->data = malloc(sizeof(points_t) * v->capacity);
    return v;
}

void vector_append(vector_xy_t *v, double xvalue, double yvalue) {
    if (v->capacity == v->size) {
        v->data = realloc(v->data, sizeof(points_t) * (2 * v->capacity));
        v->capacity *= 2;
    }
    v->data[v->size].x = xvalue;
    v->data[v->size].y = yvalue;
    v->size++;
}

void vector_free(vector_xy_t *v) {
    free(v->data);
    free(v);
}


//GX Functions
vector_xy_t *gx_rect(double width, double height) {
    vector_xy_t *points = vector_create();

    double p1x = width;
    double p1y = 0;
    double p2x = width;
    double p2y = height;
    double p3x = 0;
    double p3y = height;
    double p4x = 0;
    double p4y = 0;

    vector_append(points, p4x, p4y);
    vector_append(points, p3x, p3y);
    vector_append(points, p2x, p2y);
    vector_append(points, p1x, p1y);

    return points;
}

vector_xy_t *gx_robot(double w, double h) {
    vector_xy_t *points = vector_create();

    vector_append(points, h / 2.0, 0);
    vector_append(points, -h / 2.0, w / 2.0);
    vector_append(points, -h / 2.0, -w / 2.0);

    return points;
}

void gx_rasterize_line(vector_xy_t *pathpoints, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (1) {
        vector_append(pathpoints, x0, y0);
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

vector_xy_t *call_rasterize(vector_xy_t *points) {
    vector_xy_t *pathpoints = vector_create();
    for (int i = 0; i < points->size - 1; i++) {
        gx_rasterize_line(pathpoints, (int)points->data[i].x, (int)points->data[i].y,
                          (int)points->data[i + 1].x, (int)points->data[i + 1].y);
    }
    gx_rasterize_line(pathpoints, (int)points->data[points->size - 1].x,
                      (int)points->data[points->size - 1].y,
                      (int)points->data[0].x, (int)points->data[0].y);
    vector_free(points);
    return pathpoints;
}

void gx_rot(double theta, vector_xy_t *points) {
    for (int i = 0; i < points->size; i++) {
        double rot_x = (points->data[i].x * cos(theta) + points->data[i].y * sin(theta));
        double rot_y = (points->data[i].x * -sin(theta) + points->data[i].y * cos(theta));
        points->data[i].x = rot_x;
        points->data[i].y = rot_y;
    }
}

void gx_trans(double x, double y, vector_xy_t *points) {
    for (int i = 0; i < points->size; i++) {
        points->data[i].x += x;
        points->data[i].y += y;
    }
}

void gx_round(vector_xy_t *pathpoints) {
    double epsilon = 1e-6;
    double tempx = pathpoints->data[0].x;
    double tempy = pathpoints->data[0].y;
    for (int i = 0; i < pathpoints->size; i++) {
        if (tempx > pathpoints->data[i].x) {
            tempx = pathpoints->data[i].x;
        }
        if (tempy > pathpoints->data[i].y) {
            tempy = pathpoints->data[i].y;
        }
    }
    for (int i = 0; i < pathpoints->size; i++) {
        if (pathpoints->data[i].x == tempx) {
            pathpoints->data[i].x = ceil(pathpoints->data[i].x);
        } else {
            pathpoints->data[i].x = floor(pathpoints->data[i].x - epsilon);
        }
        if (pathpoints->data[i].y == tempy) {
            pathpoints->data[i].y = ceil(pathpoints->data[i].y);
        } else {
            pathpoints->data[i].y = floor(pathpoints->data[i].y - epsilon);
        }
    }
}

void gx_fill(bitmap_t *bmp, color_bgr_t color, vector_xy_t *pathpoints) {
    int x0[bmp->height];
    int x1[bmp->height];
    for (int i = 0; i < bmp->height; i++) {
        x0[i] = -1;
        x1[i] = -1;
    }
    for (int j = 0; j < pathpoints->size; j++) {
        int x = (int)pathpoints->data[j].x;
        int y = (int)pathpoints->data[j].y;
        if (y < 0 || y >= bmp->height) {
            continue;
        }
        if (x0[y] == -1) {
            x0[y] = x;
            x1[y] = x;
        } else {
            if (x0[y] > x) {
                x0[y] = x;
            }
            if (x0[y] < x) {
                x1[y] = x;
            }
        }
    }
    vector_free(pathpoints);

    for (int y = 0; y < bmp->height; y++) {
        if (x0[y] != -1) {
            int xmin = x0[y] > 0 ? x0[y] : 0;
            int xmax = x1[y] < (bmp->width - 1) ? x1[y] : (bmp->width - 1);
            for (int x = xmin; x <= xmax; x++) {
                bmp->data[y * bmp->width + x] = color;
            }
        }
    }
}

void gx_draw_line(bitmap_t *bmp, color_bgr_t color, int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    while (1) {
        if (x0 < 0 || x0 >= WIDTH || y0 < 0 || y0 >= HEIGHT) {
            break;
        }
        bmp->data[y0 * bmp->width + x0] = color;
        if (x0 == x1 && y0 == y1) {
            break;
        }
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void gx_draw(bitmap_t *bmp, color_bgr_t color, vector_xy_t *pathpoints) {
    for (int i = 0; i < pathpoints->size - 1; i++) {
        gx_draw_line(bmp, color, pathpoints->data[i].x, pathpoints->data[i].y,
                     pathpoints->data[i + 1].x, pathpoints->data[i + 1].y);
    }
    gx_draw_line(bmp, color, pathpoints->data[pathpoints->size - 1].x,
                 pathpoints->data[pathpoints->size - 1].y,
                 pathpoints->data[0].x, pathpoints->data[0].y);
    //vector_free(pathpoints);
}

void gx_set_backgound(bitmap_t *bmp) {
    color_bgr_t white_backdrop = {255, 255, 255};
    for (int y = 0; y < bmp->height; y++) {
        for (int x = 0; x < bmp->width; x++) {
            bmp->data[y * bmp->width + x] = white_backdrop;
        }
    }
}

void wall (bitmap_t *bmp, color_bgr_t color_sq, double x, double y) {
    vector_xy_t *points = gx_rect(BLOCK_SIZE, BLOCK_SIZE);
    gx_trans(x, y, points);
    gx_round(points);
    vector_xy_t *pathpoints = call_rasterize(points);
    gx_draw(bmp, color_sq, pathpoints);
    gx_fill(bmp, color_sq, pathpoints);
}

void robot(bitmap_t *bmp, color_bgr_t color, double x, double y, double theta) {
    vector_xy_t *points = gx_robot(ROB_W, ROB_L);
    gx_rot(theta, points);
    gx_trans(x, y, points);
    gx_round(points);
    vector_xy_t *pathpoints = call_rasterize(points);
    gx_fill(bmp, color, pathpoints);
}

void gx_draw_chaser(bitmap_t *bmp,  state_t *state) {
    color_bgr_t color_chaser = {0, 0, 255};
    vector_xy_t *points = gx_robot(ROB_W, ROB_L);
    gx_rot(state->chaser.theta, points);
    gx_trans(state->chaser.x, state->chaser.y, points); //starts at center of image
    gx_round(points);
    vector_xy_t *pathpoints = call_rasterize(points);
    gx_draw(bmp, color_chaser, pathpoints);
    gx_fill(bmp, color_chaser, pathpoints);
}

void give_runner_pos(state_t *state, int run_index) {
    int map_idx_x = run_index % MAP_W;
    int map_idx_y = run_index / MAP_W;
    state->runner.x = (map_idx_x + 0.5) * BLOCK_SIZE;
    state->runner.y = (map_idx_y + 0.5) * BLOCK_SIZE;
}

void gx_draw_runner(bitmap_t *bmp, state_t *state, int run_index) {
    if (run_index != -1) {
        give_runner_pos(state, run_index);
    }
    color_bgr_t color_runner = {0, 255, 0};
    vector_xy_t *points = gx_robot(ROB_W, ROB_L);
    gx_rot(state->runner.theta, points);
    gx_trans(state->runner.x, state->runner.y, points); //starts at center of map loc
    gx_round(points);
    vector_xy_t *pathpoints = call_rasterize(points);
    gx_draw(bmp, color_runner, pathpoints);
    gx_fill(bmp, color_runner, pathpoints);
}

void gx_draw_game(bitmap_t *bmp, state_t *state, int run_index) {
    gx_set_backgound(bmp);
    color_bgr_t color_sq = {0, 0, 0};
    for (int i = 0; i < MAP_H * MAP_W; i++) {
        if (MAP[i] == 'X') {
            //printf("index %d: %d, %d\n", i, (i % MAP_W) * 40, (int) (i/ MAP_W) * 40);
            wall (bmp, color_sq, (i % MAP_W) * BLOCK_SIZE, (int) (i / MAP_W) * BLOCK_SIZE);
        }
    }
    gx_draw_chaser(bmp, state);
    gx_draw_runner(bmp, state, run_index);
}


//Movement
void init_values(state_t *state) {
    state->runner.theta = 0;
    state->runner.x = 0;
    state->runner.y = 0;
    state->runner.is_runner = true;
    state->chaser.theta = 0;
    state->chaser.x = 320;
    state->chaser.y = 240;
    state->chaser.is_runner = false;
}

void move(robot_t *robot) {
    double fwd_vel = fmin(MAX_VEL, robot->fwd_vel);
    double xdist = fwd_vel * cos(robot->theta);
    double ydist = fwd_vel * -sin(robot->theta);
    robot->x += xdist;
    robot->y += ydist;
}

void robot_action(robot_t *robot) {
    if (robot->is_runner) { //Runner
        int num_chosen = rand() % 20; //[max = 19, min = 0]
        if (num_chosen == 1) {
            robot->fwd_vel += 2;
        } else if (num_chosen == 2) {
            robot->ang_vel += (M_PI / 16);
        }
    } else { //Chaser
        //chaser_strategy();
    }
    robot->theta += robot->ang_vel;
    robot->ang_vel *= 0.8;
    move(robot);
    resolve_tile_collision(robot);
}



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

vector_xy_t *robot2(robot_t *robot) {
    vector_xy_t *robot_vec = gx_robot(ROB_W, ROB_L);
    gx_rot(robot->theta, robot_vec);
    gx_trans(robot->x, robot->y, robot_vec);
    return robot_vec;
}

bool robots_collision(robot_t *chaser, robot_t *runner) {
    vector_xy_t *chaser_vec = robot2(chaser);
    vector_xy_t *runner_vec = robot2(runner);
    bool collides = pg_collision(chaser_vec, runner_vec);
    vector_free(runner_vec);
    vector_free(chaser_vec);
    return collides;
}

bool tile_collision(robot_t *robot, double tile_x, double tile_y) {
    //printf("In tile collision\n");
    printf("T1: %f, %f\n", tile_x, tile_y);
    // tile_x = (tile_x + 0.5) * BLOCK_SIZE;
    // tile_y = (tile_y + 0.5) * BLOCK_SIZE;
    tile_x = (tile_x * WIDTH / MAP_W);
    tile_y = (tile_y * HEIGHT / MAP_H);
    printf("T2: %f, %f\n", tile_x, tile_y);
    vector_xy_t *robot_vec = robot2(robot);
    vector_xy_t *tile_vec = gx_rect(BLOCK_SIZE, BLOCK_SIZE);
    gx_trans(tile_x, tile_y, tile_vec);
    bool collides = pg_collision(robot_vec, tile_vec);
    vector_free(tile_vec);
    vector_free(robot_vec);
    return collides;
}

void resolve_tile_collision(robot_t *robot) {
    int map_x = (robot->x * MAP_W / WIDTH);
    int map_y = (robot->y * MAP_H / HEIGHT);
    //printf("%d, %d\n", map_x, map_y);
    for (int y = map_y - 1; y <= map_y + 1; y++) {
        for (int x = map_x - 1; x <= map_x + 1; x++) {
            // for (int y = (int)fmax(map_y - 1, 0); y <= map_y + 1; y++) {
            //     for (int x = (int)fmax(map_x - 1, 0); x <= map_x + 1; x++) {
            //printf("%d, %d\n", x, y);
            if (MAP[y * 16 + x] == 'X') {
                //printf("%d, %d\n", x, y);
                if (tile_collision(robot, x, y)) {
                    //printf("%d, %d\n", x, y);
                    double dist = sqrt(pow(x * BLOCK_SIZE + 20 - robot->x, 2) +
                                       pow(y * BLOCK_SIZE + 20 - robot->y, 2));
                    double dir_x = (x * BLOCK_SIZE + 20 - robot->x) / dist;
                    double dir_y = (y * BLOCK_SIZE + 20 - robot->y) / dist;
                    robot->x -= 0.5 * dir_x;
                    robot->y -= 0.5 * dir_y;
                    if (!tile_collision(robot, x, y)) {
                        robot->fwd_vel *= 0.25;
                    }
                }
            }
        }
    }
}


// int map_idx_x = (state->runner.x / BLOCK_SIZE ) - 0.5;
// int map_idx_y = (state->runner.y / BLOCK_SIZE ) - 0.5;
// printf("Map: x, y: %d, %d\n", map_idx_x, map_idx_y);
// for (int y = map_idx_y - 1; y < map_idx_y + 1; y++) {
//     for (int x = map_idx_x - 1; x < map_idx_x + 1; x++) {
//         //printf("Map: '%c'\n", MAP[x]);
//         if (MAP[y * 16 + x] == 'X') {
//             printf("x, y: %d, %d\n", x, y);
//             //printf("Map- x, y: %d, %d\n",(i % MAP_W) * BLOCK_SIZE, (int) (i / MAP_W) * BLOCK_SIZE);
//         }
//     }
// }
