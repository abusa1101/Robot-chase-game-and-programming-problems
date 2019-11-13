#pragma once

#include <stdlib.h>
#include <termios.h>

struct termios original_termios;

typedef struct state {
    int user_action;
    robot_t chaser;
} state_t;

void reset_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
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
