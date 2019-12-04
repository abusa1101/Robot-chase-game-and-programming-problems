#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <lcm/lcm.h>
#include <time.h>
#include <pthread.h>
#include "image_server.h"
#include "bmp.h"
#include "all_functions.h"
#include "l2g_t.h"

#define HIGHLIGHT "\e[7m"
#define CLEAR_HIGHLIGHT "\e[0m"

double seconds_now(void) {
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now)) {
        fprintf(stderr, "Retrieving system time failed.\n");
        exit(1);
    }
    return now.tv_sec + now.tv_nsec / 1000000000.0;
}

void on_l2g(const lcm_recv_buf_t *rbuf, const char *channel,
            const l2g_t *msg, void *userdata) {
    settings_t *state_message = userdata;
    //printf("%.2f %.2f %.2f\n", lcm_message->l2g[0], lcm_message->l2g[1], lcm_message->l2g[2]);
}

struct settings_t {
    int64_t utime;
    int32_t initial_runner_idx;
    int32_t delay_every;
    double to_goal_magnitude;
    int32_t to_goal_power;
    double avoid_obs_magnitude;
    int32_t avoid_obs_power;
    double max_vel;
}

struct reset_t {
    int64_t utime;
    int32_t initial_runner_idx;
}

int main(void) {
    lcm_t *lcm = lcm_create(NULL);
    settings_t state_message;
    state_message.utime = 0;

    //subscribe to world
    l2g_t_subscription_t *l2g_sub = l2g_t_subscribe(lcm, "SubscribeToWorld_abusa", on_l2g, &state_message);
    l2g_t_publish(lcm, "SETTINGS_abusa", &message);

    double start = seconds_now();
    while (seconds_now() - start < 0.5) {
        lcm_handle_async(lcm);
    }

    bitmap_t bmp = {0};
    bmp_init(&bmp);
    state_t state = {0};
    init_values(&state);
    pthread_t chaser_thread;
    pthread_create(&chaser_thread, NULL, io_thread, &state);
    image_server_start("8000");
    gx_draw_game(&bmp, &state);
    state.timestep = 0;
    while (true) {
        chaser_moves(&state);
        runner_walks(&state);
        if (resolve_tile_collision(&state.chaser)) {
            state.chaser.fwd_vel *= 0.25;
        }
        if (resolve_tile_collision(&state.runner)) {
            state.runner.fwd_vel *= 0.25;
        }
        if (robots_collision(&state.chaser, &state.runner)) {
            printf("\e[2K\rRunner caught on step %d\n", state.timestep);
            reset_simulation(&state); //when chaser catches runner
            continue;
        }
        if (state.timestep % state.delay_every == 0) {
            gx_draw_game(&bmp, &state); //update gx
            serving_img(bmp, &state); //delay 40ms and all
        }
        int parameter = state.current_parameter;
        printf("\r");
        printf("%s%8.2d%s ", (parameter == 1) ? HIGHLIGHT : "",
               state.initial_runner_idx, CLEAR_HIGHLIGHT);
        printf("%s%8.2d%s ", (parameter == 2) ? HIGHLIGHT : "",
               state.delay_every, CLEAR_HIGHLIGHT);
        printf("%s%8.2f%s ", (parameter == 3) ? HIGHLIGHT : "",
               state.to_goal_magnitude, CLEAR_HIGHLIGHT);
        printf("%s%8.2d%s ", (parameter == 4) ? HIGHLIGHT : "",
               state.to_goal_power, CLEAR_HIGHLIGHT);
        printf("%s%8.2f%s ", (parameter == 5) ? HIGHLIGHT : "",
               state.avoid_obs_magnitude, CLEAR_HIGHLIGHT);
        printf("%s%8.2d%s ", (parameter == 6) ? HIGHLIGHT : "",
               state.avoid_obs_power, CLEAR_HIGHLIGHT);
        printf("%s%8.2d%s", (parameter == 7) ? HIGHLIGHT : "",
               state.max_velocity, CLEAR_HIGHLIGHT);
        fflush(stdout);
        state.timestep++;
    }
    free(bmp.data);

    l2g_t_unsubscribe(lcm, l2g_sub);
    lcm_destroy(lcm);
    return 0;
}

void *io_thread(void *user) {
    printf("\e[?25l");

    tcgetattr(0, &original_termios);
    atexit(reset_terminal);
    struct termios new_termios;
    memcpy(&new_termios, &original_termios, sizeof(new_termios));
    new_termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    state_t *state = user;
    while (true) {
        int c = getc(stdin);
        if (c == 'q') {
            exit(0);
        }
        if (c == 'r') {
            l2g_t_publish(lcm, "RESET_abusa", &message);
            reset_simulation(state); //when r is pressed
        }
        if (c == '\e' && getc(stdin) == '[') {
            c = getc(stdin);
            if (c == 'A') { //up
                update_parameters(state, 1);
            } else if (c == 'B') { //down
                update_parameters(state, 0);
            } else if (c == 'D') { //left
                state->current_parameter--;
                state->current_parameter = constrain(state->current_parameter, 1, 7);
            } else if (c == 'C') { // right
                state->current_parameter++;
                state->current_parameter = constrain(state->current_parameter, 1, 7);
            } else {
                continue;
            }
        }
    }
}
