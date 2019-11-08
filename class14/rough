#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#define TABLE_SIZE 8192
uint32_t table_hash_keys[256];

typedef struct test_entry {
    uint8_t *data;
    int n;
} test_entry_t;

typedef struct thread_info {
    int num;
    uint32_t (*hash_functions[6])(uint8_t *, int);
    int hash_i;
    uint32_t (*reduce_functions[3])(uint32_t);
    int reduce_i;
    test_entry_t *entries;
    int n_entries;
    pthread_t thread;
} thread_info_t;

void *thread_start(void *user) {
    thread_info_t *info = user;
    evaluate_hash_reduce(info.n_entries, info.entries, info.hash_functions[info.hash_i],
                         info.reduce_functions[info.reduce_i]);
    return NULL;
}

void *thread_start_main(void *user) {
    thread_info_t *info = user;
    for (int i = 0; i < N_THREADS; i++) {
        thread_infos[i].num = i;
        pthread_create(&thread_infos[i].thread, NULL, thread_start, &thread_infos[i]);
    }
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(thread_infos[i].thread, NULL);
    }
    return NULL;
}

double seconds_now(void) {
    struct timespec now;
    if (clock_gettime(CLOCK_MONOTONIC, &now)) {
        fprintf(stderr, "Retrieving system time failed.\n");
        exit(1);
    }
    return now.tv_sec + now.tv_nsec / 1000000000.0;
}

uint32_t add_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    for (int i = 0; i < n; i++) {
        hash += *data;
        data++;
    }
    return hash;
}

void setup_table_hash(void) {
    for (int i = 0; i < 256; i++) {
        table_hash_keys[i] = rand();
    }
}

uint32_t table_a_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    for (int i = 0; i < n; i++) {
        hash += table_hash_keys[*data];
        data++;
    }
    return hash;
}

uint32_t table_b_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    for (int i = 0; i < n; i++) {
        hash = (hash << 3) ^ table_hash_keys[*data];
        data++;
    }
    return hash;
}

uint32_t djb2a_hash(uint8_t *data, int n) {
    uint32_t hash = 5381;
    for (int i = 0; i < n; i++) {
        hash = ((hash << 5) + hash) ^ *data;
        data++;
    }
    return hash;
}

uint32_t fnv1a_hash(uint8_t *data, int n) {
    uint32_t FNV_offset_basis = 2166136261;
    uint32_t FNV_prime = 16777619;
    uint32_t hash = FNV_offset_basis;
    for (int i = 0; i < n; i++) {
        hash ^= *data;
        hash *= FNV_prime;
        data++;
    }
    return hash;
}

uint32_t rotate_left(uint32_t value, uint32_t count) {
    return value << count | value >> (32 - count);
}

uint32_t fxhash32_step(uint32_t hash, uint32_t value) {
    const uint32_t key = 0x27220a95;
    //const uint64_t key = 0x517cc1b727220a95;
    return (rotate_left(hash, 5) ^ value) * key;
}

uint32_t fxhash32_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    int data_chunks = n / 4;
    for (int i = 0; i < data_chunks; i++) {
        uint32_t number;
        memcpy(&number, data, sizeof(number));
        data += 4;
        hash = fxhash32_step(hash, number);
    }
    int remaining_bytes = n - (4 * data_chunks);
    for (int i = 0; i < remaining_bytes; i++) {
        hash = fxhash32_step(hash, *data);
        data += 1;
    }
    return hash;
}

uint32_t modulo2_reduce(uint32_t hash) {
    hash = hash & ((1 << 13) - 1);
    return hash;
}

uint32_t modulo_prime_reduce(uint32_t hash) {
    hash = hash % 8191;
    return hash;
}

uint32_t fibonacci32_reduce(uint32_t hash) {
    const uint32_t factor32 = 2654435769;
    hash = (uint32_t)(hash * factor32) >> (32 - 13);
    return hash;
}

void evaluate_hash_reduce(int n_entries, test_entry_t *entries,
                          uint32_t (*hash_f)(uint8_t *, int), uint32_t (*reduce_f)(uint32_t)) {
    double elapsed = 0.0;
    int loop_num = 0;
    int collision = 0;
    clock_t start = seconds_now();
    while (elapsed < 0.5) {
        int table_arr[8192] = {0};
        collision = 0;
        for (int i = 0; i < n_entries; i++) {
            loop_num++;
            uint32_t hash = reduce_f(hash_f(entries[i].data, entries[i].n));
            table_arr[hash] += 1;
            if (table_arr[hash] > 1) {
                collision++;
            }
        }
        elapsed = (seconds_now() - start) / (double)CLOCKS_PER_SEC;
    }
    elapsed = (elapsed / loop_num) * pow(10, 9);
    printf("%lfns per iteration, with %d collisions\n", elapsed, collision);
}

int main(int argc, char **argv) {
    int N_THREADS = atoi(argv[1]);
    thread_info_t thread_infos[N_THREADS];

    int max_entries = TABLE_SIZE / 2;
    thread_infos.entries = calloc(max_entries, sizeof(test_entry_t));

    for (uint16_t i = 0; i < 1000; i++) {
        entries[i].data = malloc(sizeof(i));
        memcpy(entries[i].data, &i, sizeof(i));
        entries[i].n = sizeof(i);
    }
    FILE *fp = fopen("book.txt", "r");
    char buffer[256];
    for (int i = 1000; i < max_entries; i++) {
        char *line = fgets(buffer, 256, fp);
        entries[i].data = (uint8_t *)strdup(line);
        entries[i].n = strlen(line);
    }
    setup_table_hash();

    thread_infos.n_entries = max_entries;
    thread_infos.hash_functions = {add_hash, table_a_hash, table_b_hash,
                                                    djb2a_hash, fnv1a_hash, fxhash32_hash};
    thread_infos.reduce_functions = {modulo2_reduce, modulo_prime_reduce,
                                                fibonacci32_reduce};
        mainthread_infos.num = i;
        pthread_create(&mainthread_infos.thread, NULL, mainthread_infos, &mainthread_infos);
        pthread_join(mainthread_infos.thread, NULL);


    for (int i = 0; i < max_entries; i++) {
        free(entries[i].data);
        entries[i].data = NULL;
    }
    free(thread_infos.entries);
    entries = NULL;
    return 0;
}
