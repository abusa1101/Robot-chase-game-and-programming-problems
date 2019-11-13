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
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct termios original_termios;

typedef struct state {
    int user_action;
    robot_t chaser;
} state_t;

void reset_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

void serving_img(bitmap_t bmp);
void *io_thread(void *user);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Error: Wrong number of arguments\n");
        return 1;
    }

    pthread_t thread;
    pthread_create(&thread, NULL, io_thread, NULL);

    bool disabled = false;
    if (strcmp(argv[1], "disable")) {
        disabled = true;
    }
    if (!disabled) {
        image_server_start("8000");
    }

    bitmap_t bmp = {0};
    state_t state = {0};
    bmp.width = 640;
    bmp.height = 480;
    bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));

    init_values(&state);
    while (true) {
        chaser_movement(&state);
        pthread_mutex_lock(&mutex);
        state->user_action = 0;
        pthread_mutex_unlock(&mutex);
        gx_draw_game(&bmp, &state);
        serving_img(bmp);
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

void *io_thread(void *user) {
    state_t *state = user;

    // we need to return the terminal to its original state when our program quits
    // if we don't the terminal will not behave correctly
    tcgetattr(0, &original_termios);
    atexit(reset_terminal);

    // we start with the old settings, since we are only changing a few things
    struct termios new_termios;
    memcpy(&new_termios, &original_termios, sizeof(new_termios));

    // we turn off the bits for echo (show typed characters on the screen)
    // and canonical mode, which waits until you press newline to give the program input
    new_termios.c_lflag &= ~(ECHO | ICANON);
    // we are modifying the input settings, (standard in, stdin)
    // and we want the changes to take effect right now!
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
    while (true) {
        // put the rest of your code here!
        // read characters with getc(stdin)
        int c = getc(stdin);
        printf("%c: %d\n", c, c);

        if (c == 'q') {
            exit(0);
        }
    }
}
