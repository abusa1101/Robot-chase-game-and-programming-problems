#include "bmp.h"
#include "chase_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

//Vector/low-level Operations
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

double max(double a, double b) {
    double c;
    if (a >= b) {
        c = a;
    } else {
        c = b;
    }
    return c;
}

double min(double a, double b) {
    double c;
    if (a <= b) {
        c = a;
    } else {
        c = b;
    }
    return c;
}

//Movement
void init_values(state_t *state) {
    state->chaser.theta = 0;
    state->chaser.x = 320;
    state->chaser.y = 240;
}

void move(robot_t *robot) {
    double fwd_vel = min(MAX_VEL, robot->fwd_vel);
    double xdist = fwd_vel * cos(robot->theta);
    double ydist = fwd_vel * -sin(robot->theta);
    robot->x += xdist;
    robot->y += ydist;
}

void chaser_movement(state_t *state) {
    int action = state->user_action;
    if (action == 1) { //up
        state->chaser.fwd_vel += 4;
        if (state->chaser.fwd_vel > MAX_VEL) {
            state->chaser.fwd_vel = MAX_VEL;
        }
    } else if (action == 2) { //left
        state->chaser.ang_vel -= M_PI / 32;
    } else if (action == 3) { //right
        state->chaser.ang_vel += M_PI / 32;
    }
    state->chaser.theta += state->chaser.ang_vel;
    state->chaser.ang_vel *= 0.8;
    move(&state->chaser);
    if (resolve_tile_collision(&state->chaser)) {
        state->chaser.fwd_vel *= 0.25;
    }
    state->user_action = 0;
}
