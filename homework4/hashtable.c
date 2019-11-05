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
    int entries_size;
} hashtable_t;

typedef struct test_entry {
    uint8_t *data;
    int n;
} test_entry_t;


unsigned int log2n(unsigned int n) {
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

uint32_t fibonacci32_reduce(uint32_t hash, int lognum) {
    const uint32_t factor32 = 2654435769;
    hash = (uint32_t)(hash * factor32) >> (32 - lognum);
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


hashtable_t *hashtable_create(int table_size) {
    hashtable_t *table = calloc(sizeof(hashtable_t), 1);
    table->size = table_size;
    table->entries_size = calloc(sizeof(hashtable_entry_t), table->size);
    return table;
}

void hashtable_destroy(hashtable_t *hashtable, bool free_table) {
    for (int i = 0; i < hashtable->entries_size; i++) { //or i < table size?????
        free(hashtable->entries[i]->key);
    }
    free(hashtable->entries);
    if (free_table) {
        free(hashtable);
    }
}

void hashtable_set(hashtable_t *hashtable, char *key, int value) {
    double load_factor = hashtable->entries_size / hashtable->size;
    if (load_factor >= 0.5) {
        rehash(hashtable);
    }
    int n = 7; //change this for rehashing
    int lognum = log2n(n);
    uint32_t hash = fibonacci32_reduce(fxhash32_hash(key, strlen(key)), lognum);
    bool is_slot_empty = true;
    while(is_slot_empty) {
        if (hashtable->entries[hash]->key == NULL) {
            //copy key and value and increment size by 1
            hashtable->entries[hash]->key = strdup(key);
            hashtable->entries[hash]->value = value;
            hashtable->entries_size++;
            is_slot_empty = false;
        } else { //compare keys
            if (strcmp(hashtable->entries[hash]->key, key) == 0) {
                hashtable->entries[hash]->value = value; //update entry value
                is_slot_empty = false;
            } else {
                hash++; //repeat back to step 2 so go back to while loop
            }
        }
    }
}

bool hashtable_get(hashtable_t *hashtable, char *key, int *value) {
    int n = 7;
    int lognum = log2n(n);
    uint32_t hash = fibonacci32_reduce(fxhash32_hash(key, strlen(key)), lognum);
    bool is_slot_empty = true;
    while(is_slot_empty) {
        if (hashtable->entries[hash]->key == NULL) {
            //is_slot_empty = false;
            return false;
        } else {
            if (strcmp(hashtable->entries[hash]->key, key) == 0) {
                value = hashtable->entries[hash]->value;
                //is_slot_empty = false;
                return true;
            } else {
                hash++;
            }
        }
    }
}

void rehash(hashtable_t *old_hashtable) {
    hashtable_t *new_hashtable = hashtable_create(old_hashtable->size * 2);
    for (int i = 0; i < old_hashtable->size; i++) {
        new_hashtable->entries[hash]->key = strdup(old_hashtable->entries[hash]->key);
        new_hashtable->entries[hash]->value = old_hashtable->entries[hash]->value;
        new_hashtable->entries_size++;
    }
    hashtable_destroy(old_hashtable, false); //false = DO NOT destroy hashtable_t
    new_hashtable->entries = old_hashtable->entries;
    free(new_hashtable);
}

void hashtable_probe_max(void) {}

void hashtable_probe(void) {}

void hashtable_size(void) {}

void read_word(FILE *fp, char *word, int char_num) {
    char c = fgetc(fp);
    int len = 0;
    while (c != EOF && c != ' ') {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            word[len] = c;
            len++;
        }
    }
}

int main(void) {
    FILE *fp = fopen("book.txt", "r");
    if (!fp) {
        fprintf(stderr, "Error: Missing file.\n");
        exit(1);
    }

    char *word1 = malloc(256);
    read_word(fp, word1, 256);
    char *word2 = malloc(256);
    read_word(fp, word2, 256); //do we increment fp at some point here??

    // char *buffer = malloc(256);
    int j = snprintf(word1, sizeof(word1), "%s", word2);

    fclose(fp);
    hashtable_t *hashtable = hashtable_create(TABLE_SIZE);

    // int bigram_size = hashtable_size();
    // printf("Rehashing reduced collisions from XX to XX");
    // printf("Bigram 'of the' has count of XXX");
    // printf("Total of %d different bigrams recorded", bigram_size);

    hashtable_destroy(hashtable, true); //true = destroy hashtable_t

    return 0;
}
