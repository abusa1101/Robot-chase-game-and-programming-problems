#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#define TABLE_SIZE 8192
uint32_t table_hash_keys[256];

typedef struct test_entry {
    uint8_t *data;
    int n;
} test_entry_t;

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
    clock_t start = clock();
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
        elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    }
    elapsed = (elapsed / loop_num) * pow(10, 9);
    printf("%lfns per iteration, with %d collisions\n", elapsed, collision);
}

int main(int argc, char **argv) {
    int max_entries = TABLE_SIZE / 2;
    test_entry_t *entries = calloc(max_entries, sizeof(test_entry_t));

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
    uint32_t (*hash_functions[])(uint8_t *, int) = {add_hash, table_a_hash, table_b_hash,
                                                    djb2a_hash, fnv1a_hash, fxhash32_hash};
    int n_hash_functions = sizeof(hash_functions) / sizeof(hash_functions[0]);

    uint32_t (*reduce_functions[])(uint32_t) = {modulo2_reduce, modulo_prime_reduce,
                                                fibonacci32_reduce};
    int n_reduce_functions = sizeof(reduce_functions) / sizeof(reduce_functions[0]);
    int n_entries = max_entries;
    for (int hash_i = 0; hash_i < n_hash_functions; hash_i++) {
        for (int reduce_i = 0; reduce_i < n_reduce_functions; reduce_i++) {
            evaluate_hash_reduce(n_entries, entries, hash_functions[hash_i],
                                 reduce_functions[reduce_i]);
        }
        printf("\n");
    }
    for (int i = 0; i < max_entries; i++) {
        free(entries[i].data);
        entries[i].data = NULL;
    }
    free(entries);
    entries = NULL;
    return 0;
}
