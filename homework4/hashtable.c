#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define TABLE_SIZE 128

typedef struct hashtable_entry {
    char *key;
    int value;
} hashtable_entry_t;

typedef struct hashtable {
    hashtable_entry_t entries;
    int size;
    int key;
} hashtable_t;

typedef struct test_entry {
    uint8_t *data;
    int n;
} test_entry_t;

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

uint32_t fibonacci32_reduce(uint32_t hash) {
    const uint32_t factor32 = 2654435769;
    hash = (uint32_t)(hash * factor32) >> (32 - 13);
    return hash;
}

uint32_t evaluate_hash_reduce(int n_entries, test_entry_t *entries,
                          uint32_t (*hash_f)(uint8_t *, int), uint32_t (*reduce_f)(uint32_t)) {

    int loop_num = 0;
    int collision = 0;
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
    return hash;
}

void hashtable_create(void) {
    int max_entries = TABLE_SIZE;
    hashtable_t *table = calloc(max_entries, sizeof(hashtable_t));
    table->size = TABLE_SIZE;
    table->key = 0;
}

void hashtable_set(key, value) {
    uint32_t fx_hash = fxhash32_hash();
    uint32_t fibo_hash = fibonacci32_reduce();
}

bool hashtable_get(key, value ptr) {

}
