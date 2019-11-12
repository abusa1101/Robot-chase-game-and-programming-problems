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

bool *producer_thread(void *user) {
    bool producer_finished = false;
    for (int i = 0; i < MAX_MSG; i++) { //do we do this????
        //pthread_mutex_lock(&mutex);
        buffer[i % BUFFER_CAPACITY] = i;
        buffer_size++;
        //pthread_mutex_unlock(&mutex);
        if (buffer_size == BUFFER_CAPACITY) {
            //wait a nanosecond, or as little as possible
            struct timespec interval = {0, 1}; // former number-seconds, latter-nanoseconds
            nanosleep(&interval, NULL);
        }
    }
    producer_finished = true;
    return NULL;
}

int main(int argc, char **argv) {
    bool mutex_mode = false;
    if (strcmp(argv[1], "mutex") == 0) {
        mutex_mode = true;
    }
    //Start producer thread
    pthread_create(&thread_infos[i].thread, NULL, thread_start, &thread_infos[i]);
    //Consumer thread
    while (1) {
        if (buffer_size == 0 && !producer_finished) {
            break;
        }
        if (buffer_size == BUFFER_CAPACITY) {
            printf("%d", buffer[buffer_size]);
            //pthread_mutex_lock(&mutex);
            buffer[buffer_size] = 0; //or null or what? what is popping messages off exactly?
            buffer_size -= 1;
            //pthread_mutex_unlock(&mutex);
        }

    }
    //pthread_join(thread_infos[i].thread, NULL);
    return 0;
}
