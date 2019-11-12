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
#define STR_SIZE 256

typedef struct hashtable_entry {
    char *key;
    int value;
} hashtable_entry_t;

typedef struct hashtable {
    hashtable_entry_t *entries;
    int size;
    int entries_size;
} hashtable_t;

//Declare functions
void hashtable_set(hashtable_t *hashtable, char *key, int value);

//LOW-LEVEL FUNCTIONS
int log2n(int n) {
    return (n > 1) ? 1 + log2n(n / 2) : 0;
}

uint32_t rotate_left(uint32_t value, uint32_t count) {
    return value << count | value >> (32 - count);
}

uint32_t fxhash32_step(uint32_t hash, uint32_t value) {
    const uint32_t key = 0x27220a95;
    //const uint64_t key = 0x517cc1b727220a95;
    return (rotate_left(hash, 5) ^ value) * key;
}

uint32_t fxhash32_hash(char *data, int n) {
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

uint32_t fibonacci32_reduce(uint32_t hash, int lognum) {
    const uint32_t factor32 = 2654435769;
    hash = (uint32_t)(hash * factor32) >> (32 - log2n(lognum));
    return hash;
}

//HASHTABLE FUNCTIONS
int hashtable_collisions(hashtable_t *hashtable) {
    int collision = 0;
    //int table_arr[hashtable->size] = {0};
    int *table_arr = calloc(hashtable->size, sizeof(int));
    for (int i = 0; i < hashtable->size; i++) {
        char *key = hashtable->entries[i].key;
        if (key) {
            uint32_t hash = fibonacci32_reduce(fxhash32_hash(key, strlen(key)), hashtable->size);
            table_arr[hash] += 1;
            if (table_arr[hash] > 1) {
                collision++;
            }
        }
    }
    free(table_arr);
    return collision;
}

hashtable_t *hashtable_create(int table_size) {
    hashtable_t *table = calloc(sizeof(hashtable_t), 1);
    table->size = table_size;
    table->entries = calloc(sizeof(hashtable_entry_t), table->size);
    table->entries_size = 0;
    return table;
}

void hashtable_destroy(hashtable_t *hashtable, bool free_table) {
    for (int i = 0; i < hashtable->size; i++) { //or i < table size?????
        free(hashtable->entries[i].key);
    }
    free(hashtable->entries);
    if (free_table) {
        free(hashtable);
    }
}

int hashtable_size(hashtable_t *hashtable) {
    return hashtable->entries_size;
}

void rehash(hashtable_t *old_hashtable) {
    int old_coll = hashtable_collisions(old_hashtable);
    hashtable_t *new_hashtable = hashtable_create(old_hashtable->size * 2);
    for (int i = 0; i < old_hashtable->size; i++) {
        if (old_hashtable->entries[i].key) {
            hashtable_set(new_hashtable, old_hashtable->entries[i].key, old_hashtable->entries[i].value);
            //new_hashtable->entries_size++;
        }
    }
    hashtable_destroy(old_hashtable, false);
    //false = DO NOT destroy hashtable_t. Destroy only entries
    old_hashtable->entries = new_hashtable->entries;
    old_hashtable->size = new_hashtable->size;
    old_hashtable->entries_size = new_hashtable->entries_size;
    int new_coll = hashtable_collisions(old_hashtable);
    //IS THIS SAME AS NEW_HT?
    free(new_hashtable);
    printf("Rehashing reduced collisions from %d to %d\n", old_coll, new_coll);
}

void hashtable_set(hashtable_t *hashtable, char *key, int value) {
    double load_factor = hashtable->entries_size / (double)hashtable->size;
    if (load_factor >= 0.5) {
        rehash(hashtable);
    }

    uint32_t hash = fibonacci32_reduce(fxhash32_hash(key, strlen(key)), hashtable->size);
    bool is_slot_empty = true;
    while (is_slot_empty) {
        if (!hashtable->entries[hash].key) {
            //copy key and value and increment size by 1
            hashtable->entries[hash].key = strdup(key);
            hashtable->entries[hash].value = value;
            hashtable->entries_size++;
            is_slot_empty = false;
        } else { //compare keys
            if (strcmp(hashtable->entries[hash].key, key) == 0) {
                hashtable->entries[hash].value = value; //update entry value
                is_slot_empty = false;
            } else {
                hash++; //repeat back to step 2 so go back to while loop
                if (hash >= hashtable->size) {
                    hash = 0;
                }
            }
        }
    }
}

bool hashtable_get(hashtable_t *hashtable, char *key, int *value) {
    uint32_t hash = fibonacci32_reduce(fxhash32_hash(key, strlen(key)), hashtable->size);
    bool is_slot_empty = true;
    while (is_slot_empty) {
        if (!hashtable->entries[hash].key) {
            return false; //key is not contained
        } else {
            if (strcmp(hashtable->entries[hash].key, key) == 0) {
                *value = hashtable->entries[hash].value;
                //if key matches, return the value associated with that key
                return true;
            } else {
                hash++; //there is a key but it doesnt match so look for the next empty spot
                if (hash > hashtable->size) {
                    hash = 0;
                }
            }
        }
    }
    return false;
}

int hashtable_probe_max(hashtable_t *hashtable) {
    return hashtable->size;
}

bool hashtable_probe(hashtable_t *hashtable, int i, char **key, int *value) {
    if (hashtable->entries[i].key) {
        *key = hashtable->entries[i].key;
        *value = hashtable->entries[i].value;
        return true;
    }
    return false;
}
