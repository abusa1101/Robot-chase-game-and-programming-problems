#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./graphics/bmp.h"
#include "./graphics/graphics.h"
#include "./graphics/vector.h"
#include "./graphics/vixel.h"
#include "./graphics/image_server.h"
#include <math.h>
#include "./structures/chase.h"
#include "./graphics/colors.h"
#include "./physics/collision.h"
#include "./action_lib/action_lib.h"
#include <time.h>

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
#define NEW_MAP "XXXXXXXXXXXXXXX" \
                "XX  XX XX XX XX"
#define MAP_W (WIDTH / BLOCK_SIZE)
#define MAP_H (HEIGHT / BLOCK_SIZE)
#define MAX_DEPTH 3

#define PI 3.141592653589

void draw_map(bitmap_t *bmp, char *map, polygon_t *empty_wall);
void draw_wall(bitmap_t *bmp, int x, int y, color_bgr_t color);
void draw_robot(bitmap_t *bmp, agent_t *robot);
void move_robot_to(agent_t *robot, double x, double y, double theta);
void create_wall(polygon_t *empty_wall, int idx);
int get_robot_index(agent_t *robot);
int check_collision(bitmap_t *bmp, agent_t *robot, world_t *world, int *mapping);
void random_walk(state_t *state, world_t *world, agent_t *runner);
void move_bot_away(agent_t *robot, world_t *world, float dist_to_move, int wall_index, int *mapping);
int resolve_collision(state_t *state, agent_t *robot, world_t *world);
void move_bot_fwd(agent_t *robot);
void simulate_action(state_t *state, world_t *world, int action, agent_t *robot);
bool is_collide_bots(agent_t *robot1, agent_t *robot2);
float search_actions(state_t *state, world_t *world, search_node_t node, int *action_chosen);
int choose_action(state_t *state, world_t *world, search_node_t node);
void intelligent_walk(state_t *state, world_t *world, search_node_t *node);
void slow_mode(state_t *state, world_t *world, search_node_t node, int time_steps);
void fast_mode1(state_t *state, world_t *world, search_node_t node, int time_steps);
void simulation_debugging(state_t *state, world_t *world, search_node_t *node);
int time_chase = 1;
//time_t clk;
int wall_mapping[9] = {-17, -16, -15, -1, 0, 1, 15, 16, 17};

int main(int argc, char *argv[]) {
    //srand((unsigned) time(&clk));
    int time_steps = (int)strtof(argv[1], NULL);
    int fast_mode = (int)strtof(argv[2], NULL);
    int idx = (int)strtof(argv[3], NULL);
    //int ch_x = strtof(argv[4], NULL);
    //int ch_y = strtof(argv[5], NULL);

    state_t *state = malloc(sizeof(*state));
    world_t *world = malloc(sizeof(*world));

    bitmap_t *bmp = malloc(sizeof(*bmp));
    state->bmp = bmp;
    polygon_t *wall = malloc(sizeof(*wall) * MAP_H * MAP_W);
    world->wall = wall;
    search_node_t node = {0};

    agent_t chaser = {0};
    agent_t runner = {0};
    state->chaser = chaser;
    state->runner = runner;
    world->map = MAP;
    node.depth = 0;

    state->chaser.color = red;
    state->runner.color = green;
    move_robot_to(&state->chaser, 320, 240, 0 * PI / 180);
    move_robot_to(&state->runner, (idx % MAP_W + 0.5) * BLOCK_SIZE, (idx / MAP_W + 0.5) * BLOCK_SIZE, 0 * PI / 180);
    node.chaser = state->chaser;
    node.chaser.vel = 0.01;
    node.runner.vel = 0.01;
    node.runner = state->runner;
    init_bmp(state->bmp, 640, 480);
    draw_map(state->bmp, world->map, world->wall);

    slow_mode(state, world, node, time_steps);
    //draw_robot(state->bmp, &state->chaser);
    gx_run(state->bmp);
    free(state->bmp->data);
    free(state->bmp);
    free(world->wall);
    free(state);
    free(world);
    return 0;
}

void slow_mode(state_t *state, world_t *world, search_node_t node, int time_steps) {
    int seconds = 0;
    long nanoseconds = 40 * 1000 * 1000;
    struct timespec interval = {seconds, nanoseconds};

    for (int sim_time = 0; sim_time < time_steps; sim_time++) {
        state->time_step = sim_time;
        //printf("\n\ntime step: %d\n", state->time_step);
        node.chaser = state->chaser;
        //random_walk(state, world, &state->runner);
        //intelligent_walk(state, world, &node);
        simulation_debugging(state, world, &node);
        draw_robot(state->bmp, &state->chaser);
        draw_robot(state->bmp, &state->runner);
        gx_run(state->bmp);
        nanosleep(&interval, NULL);
        if (is_collide_bots(&state->runner, &state->chaser)) {
            break;
        }
        gx_clear(state->bmp, black);
        draw_map(state->bmp, world->map, world->wall);
    }
}

void fast_mode1(state_t *state, world_t *world, search_node_t node, int time_steps) {
    for (int sim_time = 0; sim_time < time_steps; sim_time++) {
        state->time_step = sim_time;
        //printf("\n\ntime step: %d\n", state->time_step);
        node.chaser = state->chaser;
        random_walk(state, world, &state->runner);
        intelligent_walk(state, world, &node);
        if (is_collide_bots(&state->runner, &state->chaser)) {
            printf("1 1\n");
            break;
        }
    }
    draw_robot(state->bmp, &state->chaser);
    draw_robot(state->bmp, &state->runner);
    gx_run(state->bmp);
    struct timespec interval = {1, 0};
    nanosleep(&interval, NULL);

}

void simulation_debugging(state_t *state, world_t *world, search_node_t *node) {
    int action = 3;
    for (int steps = 0; steps < 10; steps++) {
      printf("%d: x: %.2f y: %.2f theta: %.2f vel: %.2f ang_vel: %.2f\n", steps, state->chaser.x, state->chaser.y, state->chaser.theta, state->chaser.vel, state->chaser.ang_vel);
      apply_action(action, &state->chaser);
      state->chaser.theta += state->chaser.ang_vel;
      state->chaser.ang_vel *= 0.8;
      move_bot_fwd(&state->chaser);
      printf("chaser pose: x:%f | y:%f | theta:%f | vel:%f\n", state->chaser.x, state->chaser.y, state->chaser.theta, state->chaser.vel);
      if (resolve_collision(state, &state->chaser, world)) {
          state->chaser.vel *= 0.25;
      }
      action = 2;
    }
}

void draw_map(bitmap_t *bmp, char *map, polygon_t *empty_wall) {
  for (int i = 0; i < strlen(map); i++) {
      if (map[i] == 'X') {
          if (time_chase == 1) {
              create_wall(empty_wall, i);
          }
          draw_wall(bmp, (i * 40) % 640 + 20, (i / 16) * 40 + 20, green);
      } else {
          //draw_wall(bmp, (i * 40) % 640 + 20, (i / 16) * 40 + 20, pink);
      }
  }
}

void draw_wall(bitmap_t *bmp, int x, int y, color_bgr_t color) {
  polygon_t *rect = gx_make_rect(bmp, BLOCK_SIZE, BLOCK_SIZE);
  gx_trans_poly(rect, x, y);
  gx_draw_poly(bmp, rect, 1, color);
  free(rect);
}

void draw_robot(bitmap_t *bmp, agent_t *robot) {
  polygon_t *triangle = gx_make_triangle(bmp, 20);
  gx_rot_poly(triangle, robot->theta);
  gx_trans_poly(triangle, robot->x, robot->y);
  //printf("%f\n", robot->x);
  gx_draw_poly(bmp, triangle, 1, robot->color);
  free(triangle);
}

void move_robot_to(agent_t *robot, double x, double y, double theta) {
    robot->x = x;
    robot->y = y;
    robot->theta = theta;
}

void create_wall(polygon_t *empty_wall, int wall_index) {
    int x_tl = (wall_index % MAP_W) * BLOCK_SIZE;
    int y_tl = (wall_index / MAP_W) * BLOCK_SIZE;
    empty_wall[wall_index].size = 4;
    empty_wall[wall_index].vertex[0][0] = x_tl;
    empty_wall[wall_index].vertex[0][1] = y_tl;
    empty_wall[wall_index].vertex[1][0] = x_tl + BLOCK_SIZE;
    empty_wall[wall_index].vertex[1][1] = y_tl;
    empty_wall[wall_index].vertex[2][0] = x_tl + BLOCK_SIZE;
    empty_wall[wall_index].vertex[2][1] = y_tl + BLOCK_SIZE;
    empty_wall[wall_index].vertex[3][0] = x_tl;
    empty_wall[wall_index].vertex[3][1] = y_tl + BLOCK_SIZE;
}

int get_robot_index(agent_t *robot) {
    int x = robot->x / BLOCK_SIZE;
    int y = robot->y / BLOCK_SIZE;
    int idx = y * MAP_W + x;
    return idx;
}

int check_collision(bitmap_t *bmp, agent_t *robot, world_t *world, int *mapping) {
    int robot_index = get_robot_index(robot);
    polygon_t *triangle = gx_make_triangle(bmp, 20);
    gx_rot_poly(triangle, robot->theta);
    gx_trans_poly(triangle, robot->x, robot->y);
    float old_x = robot->x;
    float old_y = robot->y;
    bool still_colliding = false;
    for (int row = -1; row < 2; row++) {
        for (int col = -1; col < 2; col++) {
            if (robot_index + (MAP_W * row) + col < 192) {
                if (world->map[robot_index + (MAP_W * row) + col] == 'X') {
                    if (is_collide(triangle, &world->wall[robot_index + MAP_W * row + col])
                        || is_contain(triangle, &world->wall[robot_index + MAP_W * row + col])) {
                        //printf("%d\n", is_contain(triangle, &wall[17]));
                        printf("bot_idx:%d\n%d | %d\n",robot_index, row, col);
                        move_bot_away(robot, world, 0.5, (3 * (row + 1) + (col + 2)), wall_mapping);
                        gx_trans_poly(triangle, robot->x - old_x, robot->y - old_y);
                        old_x = robot->x;
                        old_y = robot->y;
                        still_colliding = is_collide(triangle, &world->wall[robot_index + MAP_W * row + col])
                                          || is_contain(triangle, &world->wall[robot_index + MAP_W * row + col]);
                    }
                }
            }
        }
    }
    //printf("%f | %f | %f |\n", triangle->vertex[0][0], triangle->vertex[1][0], triangle->vertex[2][0]);
    free(triangle);
    if (still_colliding) {
        return 1;
    }
    return 0;
}

void random_walk(state_t *state, world_t *world, agent_t *runner) {
    int action = rand() % 20;
    //printf("%d", action);
    if (action == 1) {
        a2(runner);
        printf("1");
    } else if (action == 2) {
        a3(runner);
        printf("2");
    } else {
        a1(runner);
        printf("0");
    }
    //printf("%f | %f\n", runner->vel, runner->ang_vel);
    runner->theta += runner->ang_vel;
    runner->ang_vel *= 0.8;
    move_bot_fwd(runner);
    if (resolve_collision(state, runner, world)) {
        runner->vel *= 0.25;
    }
}

void move_bot_away(agent_t *robot, world_t *world, float dist_to_move, int wall_index, int *mapping) {
    int rob_idx = get_robot_index(robot);
    if (wall_index != 0) {
        wall_index = wall_mapping[wall_index - 1];

        int wall_x = world->wall[rob_idx + wall_index].vertex[0][0] + BLOCK_SIZE / 2;
        int wall_y = world->wall[rob_idx + wall_index].vertex[0][1] + BLOCK_SIZE / 2;
        float hx = (wall_x - robot->x);
        float hy = (wall_y - robot->y);
        //printf("%f | %f\n",hx, hy);
        if (hx != 0 || hy != 0) {
            robot->x -= (dist_to_move) * (hx / sqrt(hx  * hx + hy * hy));
            printf("%f | ", robot->x);
            robot->y += (dist_to_move) * (hy / sqrt(hx  * hx + hy * hy)); //accouting for flipped axis
            printf("%f | ", robot->y);
        } else {
            robot->x -= 1;
        }

    }
}

int resolve_collision(state_t *state, agent_t *robot, world_t *world) {
    int collision = check_collision(state->bmp, robot, world, wall_mapping);
    int resolved = collision;
    //printf("%d\n", collision);
    while (resolved) {
        //move_bot_away(robot, world, 10, collision, wall_mapping);
        resolved = check_collision(state->bmp, robot, world, wall_mapping);
        printf("Collision : %f | %f\n", robot->x, robot->y);
    }
    return collision;
}

void move_bot_fwd(agent_t *robot) {
    robot->x += robot->vel * cos(robot->theta);
    robot->y += -robot->vel * sin(robot->theta);
}

void simulate_action(state_t *state, world_t *world, int action, agent_t *robot) {
    apply_action(action, robot);
    robot->theta += robot->ang_vel;
    robot->ang_vel *= 0.8;
    move_bot_fwd(robot);
    if (resolve_collision(state, robot, world)) {
        robot->vel *= 0.25;
    }
}

bool is_collide_bots(agent_t *robot1, agent_t *robot2) {
    if (sqrt((robot1->x - robot2->x) * (robot1->x - robot2->x) +
        (robot1->y - robot2->y) * (robot1->y - robot2->y)) < 32) {
        bitmap_t *dummy_bmp = malloc(sizeof(*dummy_bmp));
        polygon_t *r1 = gx_make_triangle(dummy_bmp, 20);
        polygon_t *r2 = gx_make_triangle(dummy_bmp, 20);
        gx_rot_poly(r1, robot1->theta);
        gx_rot_poly(r2, robot2->theta);
        gx_trans_poly(r1, robot1->x, robot1->y);
        gx_trans_poly(r2, robot2->x, robot2->y);
        bool answer = is_collide(r1, r2);
        free(dummy_bmp);
        free(r1);
        free(r2);
        return answer;
    }
    //printf("%f\n", (robot1->x - robot2->x) * (robot1->x - robot2->x) +
        //(robot1->y - robot2->y) * (robot1->y - robot2->y));
    return false;
}

float search_actions(state_t *state, world_t *world, search_node_t node, int *action_chosen) {
    if (is_collide_bots(&node.chaser, &node.runner)) {
        //printf("Exit 1\n");
        return 0;
    }
    if (node.depth >= MAX_DEPTH) {
        //printf("Exit 2\n");
        return (sqrt((node.chaser.x - node.runner.x) * (node.chaser.x - node.runner.x) +
                (node.chaser.y - node.runner.y) * (node.chaser.y - node.runner.y)));
    }
    float best_score = 1000000000;
    float score = 0;
    int act_chosen = -1;
    bool chaser_found = false;
    for (int action = 1; action <= 4; action++) {
        search_node_t node1 = node;
        //printf("action number simulated: %d\n", action);
        simulate_action(state, world, action, &node1.chaser);
        int i = 0;
        while (i < 3) {
            simulate_action(state, world, 1, &node1.chaser);
            i++;
            chaser_found = is_collide_bots(&node1.chaser, &node1.runner);
            if (chaser_found) {
                //*action_chosen = action;
                return 0;
            }
            //printf("stuck here!\n");
        }
        // node.depth++;
        node1.depth += 1;
        //printf("node_depth:%d\n", node1.depth);
        score = search_actions(state, world, node1, action_chosen);
        //printf("action: %d | score:%f\n",*action_chosen, score);
        //printf("%d\n", action);
        //node.depth--;
        score += 200 / (2 < node.chaser.vel ? 2 : node1.chaser.vel);
        //printf("score with penalty:%f\n", score);
        if (score < best_score) {
            best_score = score;
            act_chosen = action;
            //printf("Depth %d: %d | %f\n",node.depth, action, best_score);
        } else {
            //printf("Depth %d: --%d | %f--\n",node.depth, action, score);
        }
        //printf("Depth %d action %d: | %f\n", node.depth, action, score);
    }
    //printf("Depth:%d | %f | action:%d\n", node.depth, best_score, act_chosen);
    *action_chosen = act_chosen;
    return best_score;
}

int choose_action(state_t *state, world_t *world, search_node_t node) {
    int action = 1;
    int *chosen_action = &action;
    float score = search_actions(state, world, node, chosen_action);
    printf(" %d\n", *chosen_action  - 1);
    return *chosen_action;
}

void intelligent_walk(state_t *state, world_t *world, search_node_t *node) {
    int action = choose_action(state, world, *node);
    apply_action(action, &state->chaser);
    state->chaser.theta += state->chaser.ang_vel;
    state->chaser.ang_vel *= 0.8;
    move_bot_fwd(&state->chaser);
    //printf("chaser pose: x:%f | y:%f | theta:%f | vel:%f\n", state->chaser.x, state->chaser.y, state->chaser.theta, state->chaser.vel);
    if (resolve_collision(state, &state->chaser, world)) {
        state->chaser.vel *= 0.25;
    }
}
