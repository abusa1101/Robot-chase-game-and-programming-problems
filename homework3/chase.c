#define _GNU_SOURCE // this must come before all the #include's in order to work

#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <float.h>

#include "graphics.h" // includes vector_xy_t.h and vector_xy_i32_t.h
#include "image_server.h"

#define max(a, b) ({__typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _a : _b; })
#define min(a, b) ({__typeof__(a) _a = (a); __typeof__(b) _b = (b); _a > _b ? _b : _a; })
#define PI 3.14159265358979323846264338327950288

#define WIDTH 640
#define HEIGHT 480

#define ROB_BASE 20
#define ROB_HEIGHT (20.0 * 4 / 3)

#define WALL_R 28.29
#define ROBOT_R 16.67
#define COLLISION_DIST_SQ pow(WALL_R + ROBOT_R, 2) // (WALL_R + ROBOT_R)^2

#define MAX_MOVEMENT 12
#define MAX_DEPTH 4
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

color_bgr_t black = {0, 0, 0};
color_bgr_t white = {255, 255, 255};
color_bgr_t red = {0, 0, 255};
color_bgr_t green = {0, 255, 0};

typedef struct agent {
    bool is_runner;
    double x;
    double y;
    double theta;
    double vel;
    double ang_vel;
} agent_t;

typedef struct state {
    int depth;
    agent_t runner;
    agent_t chaser;
} state_t;

bool resolve_collision(agent_t *agent);
double search_actions(state_t node, int *chosen_action);

int sign(double m) {
    int sign;
    if (m > 0) {
        sign = 1;
    } else if (m < 0) {
        sign = -1;
    } else {
        sign = 0;
    }
    return sign;
}

int cross_ll(line_t l1, line_t l2) {
    int flag = 0;
    point_t p1;
    point_t p2;
    point_t q1;
    point_t q2;

    p1.x = l1.x0;
    p1.y = l1.y0;

    p2.x = l1.x1;
    p2.y = l1.y1;

    q1.x = l2.x0;
    q1.y = l2.y0;

    q2.x = l2.x1;
    q2.y = l2.y1;

    double cross1 = (q1.y - p1.y) * (p2.x - p1.x) - (q1.x - p1.x) * (p2.y - p1.y);
    int sign1 = sign(cross1);
    double cross2 = (q2.y - p1.y) * (p2.x - p1.x) - (q2.x - p1.x) * (p2.y - p1.y);
    int sign2 = sign(cross2);
    //printf("1, 2: %d, %d\n", sign1, sign2);
    int sign_a = sign1 * sign2;

    double cross3 = (p1.y - q1.y) * (q2.x - q1.x) - (p1.x - q1.x) * (q2.y - q1.y);
    int sign3 = sign(cross3);
    double cross4 = (p2.y - q1.y) * (q2.x - q1.x) - (p2.x - q1.x) * (q2.y - q1.y);
    int sign4 = sign(cross4);
    //printf("3, 4: %d, %d\n", sign3, sign4);
    int sign_b = sign3 * sign4;
    //print_line(l1); print_line(l2); printf("%d, %d\n", sign_a, sign_b);

    if (sign_a == -1 && sign_b == -1) {
        flag = 1;
    }

    if (sign_a == 0 && sign_b == -1) {
        flag = 1;
    }

    if (sign_a == -1 && sign_b == 0) {
        flag = 1;
    }
    //printf("Flag: %d\n", flag);
    return flag;
}

int containing(line_t l, double x, double y) {
    double cross = (l.x1 - l.x0) * (y - l.y0) - (x - l.x0) * (l.y1 - l.y0);
    return sign(cross);
}

int touching(line_t l, double x, double y) {
    double cross = (l.x1 - l.x0) * (y - l.y0) - (x - l.x0) * (l.y1 - l.y0);
    double max_x = (l.x0 > l.x1) ? l.x0 : l.x1;
    double max_y = (l.y0 > l.y1) ? l.y0 : l.y1;
    double min_x = (l.x0 < l.x1) ? l.x0 : l.x1;
    double min_y = (l.y0 < l.y1) ? l.y0 : l.y1;
    if (cross == 0) {
        if (min_x <= x && x <= max_x && min_y <= y && y <= max_y) {
            return 1;
        }
    }
    return 0;
}

int collision(vector_xy_t v1, vector_xy_t v2) {
    line_t lines_a[v1.size];
    line_t lines_b[v2.size];
    point_t points_a[v1.size];
    point_t points_b[v2.size];

    for (int i = 0; i < v1.size; i++) {
        if (i + 1 == v1.size) {
            lines_a[i].x0 = v1.data[i].x;
            lines_a[i].x1 = v1.data[0].x;
            lines_a[i].y0 = v1.data[i].y;
            lines_a[i].y1 = v1.data[0].y;
        } else {
            lines_a[i].x0 = v1.data[i].x;
            lines_a[i].x1 = v1.data[i + 1].x;
            lines_a[i].y0 = v1.data[i].y;
            lines_a[i].y1 = v1.data[i + 1].y;
        }
    }
    for (int i = 0; i < v2.size; i++) {
        if (i + 1 == v2.size) {
            lines_b[i].x0 = v2.data[i].x;
            lines_b[i].x1 = v2.data[0].x;
            lines_b[i].y0 = v2.data[i].y;
            lines_b[i].y1 = v2.data[0].y;
        } else {
            lines_b[i].x0 = v2.data[i].x;
            lines_b[i].x1 = v2.data[i + 1].x;
            lines_b[i].y0 = v2.data[i].y;
            lines_b[i].y1 = v2.data[i + 1].y;
        }
    }

    for (int i = 0; i < v1.size; i++) {
        for (int j = 0; j < v2.size; j++) {
            int ll_intersect = cross_ll(lines_a[i], lines_b[j]);
            if (ll_intersect == 1) {
                return ll_intersect;
            }
            int onLine = touching(lines_b[j], v1.data[i].x, v1.data[i].y);
            if (onLine == 1) {
                return onLine;
            }
        }
    }

    for (int i = 0; i < v1.size; i++) {
        int l_c[v2.size];
        int in = 0;
        for (int j = 0; j < v2.size; j++) {
            l_c[j] = containing(lines_b[j], v1.data[i].x, v1.data[i].y);
            in += l_c[j];
        }
        if (in == v2.size || in == -v2.size) {
            return 1;
        }
    }

    for (int i = 0; i < v2.size; i++) {
        int l_c[v1.size];
        int in = 0;
        for (int j = 0; j < v1.size; j++) {
            l_c[j] = containing(lines_a[j], v2.data[i].x, v2.data[i].y);
            in += l_c[j];
        }
        if (in == v1.size || in == -v1.size) {
            return 1;
        }
    }
    return 0;
}

vector_xy_t poly_robot(agent_t *rob) {
    vector_xy_t v4 = vector_create_xy();
    gx_robot(ROB_BASE, ROB_HEIGHT, &v4);
    gx_rot(rob->theta, &v4);
    gx_trans(rob->x, rob->y, &v4);

    return v4;
}

void bmp_init(bitmap_t *bmp) {
    bmp->width = WIDTH;
    bmp->height = HEIGHT;
    bmp->data = calloc(bmp->width * bmp->height, sizeof(color_bgr_t));
    for (int i = 0; i < bmp->height * bmp->width; i++) {
        bmp->data[i] = white;
    }
}

void build_map(bitmap_t *bmp) {
    for (int i = 0; i < bmp->height * bmp->width; i++) {
        bmp->data[i] = white;
    }
    for (int i = 0; i < MAP_H; i++) {
        for (int j = 0; j < MAP_W; j++) {
            vector_xy_t vec = vector_create_xy();
            if (MAP[i * MAP_W + j] == 'X') {
                // printf("%c", MAP[i * MAP_W + j]);
                gx_rect(BLOCK_SIZE, BLOCK_SIZE, &vec);
                gx_trans((j + 0.5) * BLOCK_SIZE, (i + 0.5) * BLOCK_SIZE, &vec);
                gx_fill(bmp, black, &vec);
            }
            vector_destroy_xy(&vec);
        }
    }
}

void chaser_runner_init(bitmap_t *bmp, state_t *state, int initial_runner_idx) {
    // Chaser
    vector_xy_t v = vector_create_xy();
    gx_robot(ROB_BASE, ROB_HEIGHT, &v);
    state->chaser.x = 320;
    state->chaser.y = 240;
    gx_trans(state->chaser.x, state->chaser.y, &v);
    gx_fill(bmp, red, &v);
    vector_destroy_xy(&v);

    // Runner
    int map_idx_x = initial_runner_idx % MAP_W;
    int map_idx_y = initial_runner_idx / MAP_W;
    state->runner.x = (map_idx_x + 0.5) * BLOCK_SIZE;
    state->runner.y = (map_idx_y + 0.5) * BLOCK_SIZE;

    vector_xy_t v2 = vector_create_xy();
    gx_robot(ROB_BASE, ROB_HEIGHT, &v2);
    gx_trans(state->runner.x, state->runner.y, &v2);
    gx_fill(bmp, green, &v2);
    vector_destroy_xy(&v2);
}

void chaser_runner_update(bitmap_t *bmp, state_t *state) {
    // Chaser
    vector_xy_t v = vector_create_xy();
    gx_robot(ROB_BASE, ROB_HEIGHT, &v);
    gx_rot(state->chaser.theta, &v);
    gx_trans(state->chaser.x, state->chaser.y, &v);
    //printf("p_chaser : %lf %lf %lf\n", state->chaser.x, state->chaser.y, state->chaser.theta);
    gx_fill(bmp, red, &v);
    vector_destroy_xy(&v);

    // Runner
    vector_xy_t v2 = vector_create_xy();
    gx_robot(ROB_BASE, ROB_HEIGHT, &v2);
    gx_rot(state->runner.theta, &v2);
    gx_trans(state->runner.x, state->runner.y, &v2);
    gx_fill(bmp, green, &v2);

    vector_destroy_xy(&v2);
}

void robot_action(agent_t *agent, int choice) {
    switch (choice) {
    case 1:
        agent->vel += 2;
        if (agent->vel > 12) {
            agent->vel = MAX_MOVEMENT;
        }
        break;
    case 2:
        agent->ang_vel += PI / 16;
        break;
    case 3:
        if (!agent->is_runner) {
            agent->ang_vel -= PI / 16;
        }
        break;
    default:
        break;
    }

    agent->theta += agent->ang_vel;
    agent->ang_vel *= 0.8;
    agent->x += agent->vel * cos(agent->theta);
    agent->y += agent->vel * -sin(agent->theta);
}

void runner_random_walk(state_t *state) {
    int random = rand() % 20;
    robot_action(&state->runner, random);
    if (random == 1 || random == 2) {
        printf("%d", random);
    } else {
        printf("%d", 0);
    }
}

int found_runner(state_t *state) {
    vector_xy_t chaser = poly_robot(&state->chaser);
    vector_xy_t runner = poly_robot(&state->runner);
    if (collision(chaser, runner)) {
        vector_destroy_xy(&runner);
        vector_destroy_xy(&chaser);
        return true;
    }
    vector_destroy_xy(&runner);
    vector_destroy_xy(&chaser);
    return false;
}

void robot_movement(state_t *state) {
    runner_random_walk(state);
    if (resolve_collision(&state->runner)) {
        state->runner.vel *= 0.25;
    }

    // printf("Runner x y : %.2f, %.2f\n ", state->runner.x, state->runner.y);
    // printf("ys : %.2f, %.2f, %.2f\n", robot.data[0].y, robot.data[1].y,
    int action = 0;
    state_t state_copy = *state;
    state_copy.depth = 0;
    search_actions(state_copy, &action);
    //double score = search_actions(state_copy, &action);
    //printf(" %lf", score);
    printf(" %d\n", action);

    //printf("x: %.2f y: %.2f theta: %.2f vel: %.2f ang_vel: %.2f\n", state->chaser.x,
    //state->chaser.y, state->chaser.theta, state->chaser.vel, state->chaser.ang_vel);
    robot_action(&state->chaser, action);
    // robot_action(&state->chaser, 1);
    // printf("x: %lf y: %lf theta: %lf ang: %lf ang_vel: %lf\n", agent->x, agent->y,
    //agent->theta, agent->vel, agent->ang_vel);
    if (resolve_collision(&state->chaser)) {
        state->chaser.vel *= 0.25;
    }
    //printf("Chaser x y : %.2f, %.2f\n", state->chaser.x, state->chaser.y);
}

bool resolve_collision(agent_t *agent) {
    // Checks collision with walls
    // printf("agent is chaser? : %d\n", !agent->is_runner);
    vector_xy_t robot = poly_robot(agent);
    bool any_collision = false;
    bool rot_wall = true;

    // Select Walls
    while (rot_wall) {
        rot_wall = false;
        for (int wall_y = 0; wall_y < MAP_H; wall_y++) {
            for (int wall_x = 0; wall_x < MAP_W; wall_x++) {
                if (MAP[wall_y * MAP_W + wall_x] == 'X') {
                    double cent_wall_x = (wall_x + 0.5) * BLOCK_SIZE;
                    double cent_wall_y = (wall_y + 0.5) * BLOCK_SIZE;

                    double dist_x = cent_wall_x - agent->x;
                    double dist_y = cent_wall_y - agent->y;
                    double dist_x2 = pow(dist_x, 2);
                    double dist_y2 = pow(dist_y, 2);
                    double dist_sq = dist_x2 + dist_y2;

                    if (dist_sq <= COLLISION_DIST_SQ) {
                        vector_xy_t selected_wall = vector_create_xy();
                        gx_rect(BLOCK_SIZE, BLOCK_SIZE, &selected_wall); //Centered at 0,0
                        //Move by wall_x, wall_y and Offset by BLOCK_SIZE
                        gx_trans(cent_wall_x, cent_wall_y, &selected_wall);
                        // printf("Collision w/ tile %d, %d", wall_x, wall_y);
                        if (collision(robot, selected_wall)) {
                            // printf("movement from %.2f, %.2f to ", agent->x, agent->y);
                            any_collision = true;
                            rot_wall = true;
                            agent->x -= 0.5 * dist_x / sqrt(dist_sq);
                            agent->y -= 0.5 * dist_y / sqrt(dist_sq);
                            // printf(" %.2f, %.2f", agent->x, agent->y);
                            // printf(" with robot xs : %.2f, %.2f, %.2f ",robot.data[0].x,
                            //robot.data[1].x, robot.data[2].x);
                            // printf("ys : %.2f, %.2f, %.2f\n", robot.data[0].y, robot.data[1].y,
                            //robot.data[2].y);
                            vector_destroy_xy(&robot);
                            robot = poly_robot(agent);
                        }
                        vector_destroy_xy(&selected_wall);
                    }
                }
            }
        }
    }

    vector_destroy_xy(&robot);
    //printf("collides: %d\n", c);
    return any_collision;
}

bool robot_found(state_t node) {
    vector_xy_t node_chaser = poly_robot(&node.chaser);
    vector_xy_t node_runner = poly_robot(&node.runner);
    if (collision(node_chaser, node_runner)) {
        vector_destroy_xy(&node_runner);
        vector_destroy_xy(&node_chaser);
        return false;
    }
    vector_destroy_xy(&node_runner);
    vector_destroy_xy(&node_chaser);
    return true;
}

double search_actions(state_t node, int *chosen_action) {
    if (!robot_found(node)) {
        return 0;
    }

    if (node.depth >= MAX_DEPTH) {
        double score_x = pow(node.runner.x - node.chaser.x, 2);
        double score_y = pow(node.runner.y - node.chaser.y, 2);
        double score_cent = sqrt(score_x + score_y);

        return score_cent;
    }

    double score[4];
    // There are 4 chaser actions
    for (int i = 0; i < 4; i++) {
        state_t new_node = node;
        vector_xy_t new_node_chaser = poly_robot(&new_node.chaser);
        vector_xy_t new_node_runner = poly_robot(&new_node.runner);
        // while (collision(poly_robot(new_node->chaser), poly_robot(new_node->runner)) {
        robot_action(&new_node.chaser, i);
        // resolve_collision(&new_node.chaser);
        if (resolve_collision(&new_node.chaser)) {
            new_node.chaser.vel *= 0.25;
        }
        for (int j = 0; j < 3; j++) {
            if (!collision(new_node_chaser, new_node_runner)) {
                robot_action(&new_node.chaser, 0); //do nothing
                if (resolve_collision(&new_node.chaser)) {
                    new_node.chaser.vel *= 0.25;
                }
            }
        }
        new_node.depth++;
        score[i] = search_actions(new_node, chosen_action);
        score[i] += 200 / min(2, new_node.chaser.vel);
        //printf("depth : %d: score[%d] :%lf\n", new_node.depth, i, score[i]);
        vector_destroy_xy(&new_node_runner);
        vector_destroy_xy(&new_node_chaser);
    }

    double best_score = 10000;
    for (int j = 0; j < 4; j++) {
        if (score[j] < best_score) {
            best_score = score[j];
            *chosen_action = j;
        }
    }
    return best_score;
}

void update_graphics(bitmap_t *bmp, state_t *state) {
    build_map(bmp);
    chaser_runner_update(bmp, state);

    size_t bmp_size = bmp_calculate_size(bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(bmp, serialized_bmp);
    // serialized_bmp now has the full bmp formatted image

    // make image_data a valid bmp image file
    image_server_set_data(bmp_size, serialized_bmp);
    // write to a file so we can check if it is a valid image
    FILE *f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    free(serialized_bmp);
}



int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "usage: usage: ./chase <time steps> <fast=0|1> <initial runner index>\n");
        return 1;
    }

    int time_steps = atoi(argv[1]);
    int speed = atoi(argv[2]);
    int initial_runner_idx = atoi(argv[3]);

    bitmap_t bmp = {0};
    bmp_init(&bmp);
    if (speed != 2) {
        image_server_start("8000");
    }

    state_t robots = {0};
    robots.runner.is_runner = true;

    build_map(&bmp);
    chaser_runner_init(&bmp, &robots, initial_runner_idx);

    for (int i = 1; i <= time_steps; i++) {
        if (!speed) {
            robot_movement(&robots);
            update_graphics(&bmp, &robots);
            int seconds = 0;
            long nanoseconds = 40 * 1000 * 1000;
            struct timespec interval = {seconds, nanoseconds};
            nanosleep(&interval, NULL);
        } else if (speed == 1) {
            robot_movement(&robots);
            update_graphics(&bmp, &robots);
        } else if (speed == 2) {
            robot_movement(&robots);
        }
        if (found_runner(&robots)) {
            break;
        }
    }

    free(bmp.data);
    return 0;
}
