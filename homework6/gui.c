#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <lcm/lcm.h>
#include <time.h>
#include "l2g_t.h"

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

int main(void) {
    // we could also pass in a string of settings for special LCM configuration
    // and that would be helpful to configure a multi-robot communication scenario
    lcm_t *lcm = lcm_create(NULL);
    settings_t state_message;

    state_message.utime = 0;

    l2g_t_subscription_t *l2g_sub = l2g_t_subscribe(lcm, "SETTINGS_subscribe", on_l2g, &state_message);
    l2g_t_publish(lcm, "SETTINGS_abusa", &message);

    double start = seconds_now();
    while (seconds_now() - start < 0.5) {
        //lcm_handle(lcm);
        //lcm_handle_timeout(lcm, 100);
        lcm_handle_async(lcm);
    }

    int parameter = state_message.current_parameter;
    printf("\r");
    printf("%s%8.2d%s ", (parameter == 1) ? HIGHLIGHT : "",
           state_message.initial_runner_idx, CLEAR_HIGHLIGHT);
    printf("%s%8.2d%s ", (parameter == 2) ? HIGHLIGHT : "",
           state_message.delay_every, CLEAR_HIGHLIGHT);
    printf("%s%8.2f%s ", (parameter == 3) ? HIGHLIGHT : "",
           state_message.to_goal_magnitude, CLEAR_HIGHLIGHT);
    printf("%s%8.2d%s ", (parameter == 4) ? HIGHLIGHT : "",
           state_message.to_goal_power, CLEAR_HIGHLIGHT);
    printf("%s%8.2f%s ", (parameter == 5) ? HIGHLIGHT : "",
           state_message.avoid_obs_magnitude, CLEAR_HIGHLIGHT);
    printf("%s%8.2d%s ", (parameter == 6) ? HIGHLIGHT : "",
           state_message.avoid_obs_power, CLEAR_HIGHLIGHT);
    printf("%s%8.2d%s", (parameter == 7) ? HIGHLIGHT : "",
           state_message.max_velocity, CLEAR_HIGHLIGHT);
    fflush(stdout);

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
