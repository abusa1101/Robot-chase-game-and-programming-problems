#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#define BUFFER_CAPACITY 8
#define MAX_MSG 30
int buffer[BUFFER_CAPACITY];
int buffer_size = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
bool producer_finished = false;
bool mutex_mode = false;

void *producer_thread(void *user) {
    int msg = 0;
    while (msg < MAX_MSG) { //do we do this????
        if (buffer_size < BUFFER_CAPACITY) {
            if (mutex_mode) {
                pthread_mutex_lock(&mutex);
                buffer[buffer_size] = msg;
                buffer_size++;
                pthread_mutex_unlock(&mutex);
            } else {
                buffer[buffer_size] = msg;
                buffer_size++;
            }
            msg++;
            while (buffer_size >= BUFFER_CAPACITY) {
                //wait a nanosecond, or as little as possible
                struct timespec interval = {0, 1}; //former number-seconds, latter-nanoseconds
                nanosleep(&interval, NULL);
            }
        }
    }
    producer_finished = true;
    return NULL;
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "mutex") == 0) {
        mutex_mode = true;
    }
    //Start producer thread
    pthread_t thread;
    pthread_create(&thread, NULL, producer_thread, NULL);
    //Consumer thread
    while (1) {
        if (buffer_size > 0) {
            if (mutex_mode) {
                pthread_mutex_lock(&mutex);
            }
            buffer_size--;
            //printf("BS: %d\n", buffer_size);
            printf("%d\n", buffer[buffer_size]);
            if (mutex_mode) {
                pthread_mutex_unlock(&mutex);
            }
            if (buffer_size == 0 && producer_finished) {
                break;
            }
        }
    }
    return 0;
}
