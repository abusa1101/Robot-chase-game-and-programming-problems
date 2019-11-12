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
    // printf("here in PT\n");
    for (int i = 0; i < MAX_MSG; i++) { //do we do this????
        //printf("BS,MAX_MSG: %d, %d,\n", buffer_size, MAX_MSG);
        if (mutex_mode) {
            pthread_mutex_lock(&mutex);
        }
        buffer[i % BUFFER_CAPACITY] = i;
        buffer_size++;
        if (mutex_mode) {
            pthread_mutex_unlock(&mutex);
        }
        if (buffer_size == BUFFER_CAPACITY) {
            // printf("here in if size equal\n");
            //wait a nanosecond, or as little as possible
            struct timespec interval = {0, 1}; // former number-seconds, latter-nanoseconds
            nanosleep(&interval, NULL);
        }
    }
    producer_finished = true;
    // printf("here PF is true now\n");
    return NULL;
}

int main(int argc, char **argv) {
    if (argv[1] != NULL) {
        if (strcmp(argv[1], "mutex") == 0) {
            mutex_mode = true;
        }
    }

    // printf("here in main\n");
    //Start producer thread
    pthread_t thread;
    pthread_create(&thread, NULL, producer_thread, NULL);
    // printf("here after create\n");
    //Consumer thread
    while (1) {
        if (buffer_size > 0) {
            if (mutex_mode) {
                pthread_mutex_lock(&mutex);
            }
            buffer_size --;
            printf("%d\n", buffer[buffer_size]);
            if (mutex_mode) {
                pthread_mutex_unlock(&mutex);
            }
            if (buffer_size == 0 && producer_finished) {
                // printf("here in break while\n");
                break;
            }
        }

    }
    // printf("here broke out of while\n");
    //pthread_join(thread_infos[i].thread, NULL);
    return 0;
}
