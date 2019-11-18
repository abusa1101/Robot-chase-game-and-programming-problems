#define _GNU_SOURCE
#include <unistd.h>
#include <time.h>
#include "image_server.h"
#include "bmp.h"
#include "chase_functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>
#include <termios.h>

struct termios original_termios;

void serving_img(bitmap_t bmp);
void reset_terminal(void);
void *io_thread(void *user);

int main(int argc, char **argv) {
    // if (argc != 2) {
    //     fprintf(stderr, "Error: Wrong number of arguments\n");
    //     return 1;
    // }

    pthread_t chaser_thread;
    bitmap_t bmp = {0};
    state_t state = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
    init_values(&state);

    pthread_create(&chaser_thread, NULL, io_thread, &state);
    gx_draw_game(&bmp, &state);
    while (true) {
        image_server_start("8000");
        chaser_movement(&state);
        gx_draw_game(&bmp, &state);
        serving_img(bmp);
        printf("%.2f %.2f\n", state.chaser.fwd_vel, state.chaser.ang_vel);
    }

    free(bmp.data);
    return 0;
}

void serving_img(bitmap_t bmp) {
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_serialize(&bmp, serialized_bmp);
    image_server_set_data(bmp_size, serialized_bmp);
    free(serialized_bmp);
    int seconds = 0;
    long nanoseconds = 40 * 1000 * 1000;
    struct timespec interval = {seconds, nanoseconds};
    nanosleep(&interval, NULL);
}

void reset_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

void *io_thread(void *user) {
    state_t *state = user;
    tcgetattr(0, &original_termios);
    atexit(reset_terminal);
    struct termios new_termios;
    memcpy(&new_termios, &original_termios, sizeof(new_termios));
    new_termios.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    while (true) {
        int c = getc(stdin);
        if (c == 'q') {
            exit(0);
        }
        if (c == '\e' && getc(stdin) == '[') {
            c = getc(stdin);
            if (c == 'A') {
                state->user_action = 1; //up
            } else if (c == 'C') {
                state->user_action = 2; //left
            } else if (c == 'D') {
                state->user_action = 3; // right
            } else {
                continue;
            }
        }
    }
}
