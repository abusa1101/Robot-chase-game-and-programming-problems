#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#define TABLE_SIZE 128

// typedef struct hashtable hashtable_t;

typedef struct hashtable_entry {
    char *key;
    int value;
} hashtable_entry_t;

typedef struct hashtable {
    hashtable_entry_t entries;
    int size;
    int entries_size;
} hashtable_t;


int log2n(int n);
uint32_t rotate_left(uint32_t value, uint32_t count);
uint32_t fxhash32_step(uint32_t hash, uint32_t value);

uint32_t fxhash32_hash(uint8_t *data, int n);
uint32_t fibonacci32_reduce(uint32_t hash, int lognum);
hashtable_t *hashtable_create(int table_size);
void hashtable_destroy(hashtable_t *hashtable, bool free_table);
void hashtable_set(hashtable_t *hashtable, char *key, int value);
bool hashtable_get(hashtable_t *hashtable, char *key, int *value);
void rehash(hashtable_t *old_hashtable);
// void read_word(FILE *fp, char *word, int char_num);
