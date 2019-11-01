#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>

//Naive "add" hash
uint32_t add_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    for (int i = 0; i < n; i++) {
        hash += *data;
        data++;
    }
    return hash;
}

//Naive/Better table hash
uint32_t table_hash_keys[256];

void setup_table_hash(void) {
    for (int i = 0; i < 256; i++) {
        table_hash_keys[i] = rand();
    }
}

uint32_t table_a_hash(uint8_t *data, int n) {
    uint32_t hash = 0;
    setup_table_hash();
    for (int i = 0; i < n; i++) {
        hash += table_hash_keys[*data];
        data++;
    }
    return hash;
}

//Better Hash Table
uint32_t table_b_hash(uint8_t *data, int n) {
    setup_table_hash();
    uint32_t hash = 0;
    for (int i = 0; i < n; i++) {
        hash = (hash << 3) ^ table_hash_keys[*data];
        data++;
    }
    return hash;
}

//DJB2A
uint32_t DJB2A(uint8_t *data, int n) {
    setup_table_hash();
    uint32_t hash = 5381;
    for (int i = 0; i < n; i++) {
        hash = ((hash << 5) + hash) ^ *data;
        data++;
    }
    return hash;
}

//FNV1A
uint32_t FNV1A(uint8_t *data, int n) {
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

//Fxhash32
uint32_t rotate_left(uint32_t value, uint32_t count) {
    return value << count | value >> (32 - count);
}

uint32_t fxhash32_step(uint32_t hash, uint32_t value) {
    const uint32_t key = 0x27220a95;
    //const uint64_t key = 0x517cc1b727220a95;
    return (rotate_left(hash, 5) ^ value) * key;
}

uint32_t fxhash32(uint8_t *data, int n) {
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

int main(int argc, char **argv) {
    if (argc != 3) { //No of arguments error
        fprintf(stderr, "usage: %s <file> <key> <lines before>\n", argv[0]);
        return 1;
    }

    char *func = argv[1];

    uint8_t *data = (uint8_t *)argv[2];
    int n = (int)strlen(argv[2]);

    if (strcmp(func, "add") == 0) {
        uint32_t hash_add = add_hash(data, n);
        printf("0x%x\n", hash_add);
    } else if (strcmp(func, "table_a") == 0) {
        uint32_t hash_a = table_a_hash(data, n);
        printf("0x%x\n", hash_a);
    } else if (strcmp(func, "table_b") == 0) {
        uint32_t hash_b = table_b_hash(data, n);
        printf("0x%x\n", hash_b);
    } else if (strcmp(func, "djb2a") == 0) {
        uint32_t hash_dj = DJB2A(data, n);
        printf("0x%x\n", hash_dj);
    } else if (strcmp(func, "fnv1a") == 0) {
        uint32_t hash_fn = FNV1A(data, n);
        printf("0x%x\n", hash_fn);
    } else if (strcmp(func, "fxhash32") == 0) {
        uint32_t hash_fx32 = fxhash32(data, n);
        printf("0x%x\n", hash_fx32);
    } else {
        printf("Error: Invalid Function");
    }
    return 0;
}
