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

// uint32_t evaluate_hash_reduce(int n_entries, test_entry_t *entries,
//                           uint32_t (*hash_f)(uint8_t *, int), uint32_t (*reduce_f)(uint32_t)) {
//
//     int loop_num = 0;
//     int collision = 0;
//     int table_arr[8192] = {0};
//     collision = 0;
//     for (int i = 0; i < n_entries; i++) {
//         loop_num++;
//         uint32_t hash = reduce_f(hash_f(entries[i].data, entries[i].n));
//         table_arr[hash] += 1;
//         if (table_arr[hash] > 1) {
//             collision++;
//         }
//     }
//     return hash;
// }

hashtable_t *hashtable_create(int table_size) {
    hashtable_t *table = calloc(sizeof(hashtable_t), 1);
    table->size = table_size;
    table->entries = calloc(sizeof(hashtable_entry_t), table->size);
    table->entries_size = 0;
    return table;
}

void hashtable_destroy(hashtable_t *hashtable, bool free_table) {
    for (int i = 0; i < hashtable->entries_size; i++) { //or i < table size?????
        free(hashtable->entries[i].key);
    }
    free(hashtable->entries);
    if (free_table) {
        free(hashtable);
    }
}

void rehash(hashtable_t *old_hashtable) {
    hashtable_t *new_hashtable = hashtable_create(old_hashtable->size * 2);
    for (int i = 0; i < old_hashtable->size; i++) {
        new_hashtable->entries[i].key = strdup(old_hashtable->entries[i].key);
        new_hashtable->entries[i].value = old_hashtable->entries[i].value;
        new_hashtable->entries_size++;
    }
    hashtable_destroy(old_hashtable, false); //false = DO NOT destroy hashtable_t
    new_hashtable->entries = old_hashtable->entries;
    free(new_hashtable);
}

void hashtable_set(hashtable_t *hashtable, char *key, int value) {
    double load_factor = hashtable->entries_size / hashtable->size;
    if (load_factor >= 0.5) {
        rehash(hashtable);
    }

    uint32_t hash = fibonacci32_reduce(fxhash32_hash(key, strlen(key)), hashtable->size);
    bool is_slot_empty = true;
    while(is_slot_empty) {
        if (hashtable->entries[hash].key == NULL) {
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
            }
        }
    }
}

bool hashtable_get(hashtable_t *hashtable, char *key, int *value) {
    uint32_t hash = fibonacci32_reduce(fxhash32_hash(key, strlen(key)), hashtable->size);
    bool is_slot_empty = true;
    while(is_slot_empty) {
        if (hashtable->entries[hash].key == NULL) {
            //is_slot_empty = false;
            return false;
        } else {
            if (strcmp(hashtable->entries[hash].key, key) == 0) {
                *value = hashtable->entries[hash].value;
                //is_slot_empty = false;
                return true;
            } else {
                hash++;
            }
        }
    }
    return false;
}

// int hashtable_collisions(hashtable_t *hashtable) {}

// void hashtable_probe_max(void) {}
//
// void hashtable_probe(void) {}
//
// void hashtable_size(void) {}

void read_word(FILE *fp, char *word, int char_num) {
    int len = 0;
    while (1) {
        char c = fgetc(fp);
        if (feof(fp) || c == ' ') {
            word[len] = '\0';
            //printf("%c", c);
            return;
        }
        if (isalpha(c)) {
            //printf("%c", c);
            word[len] = c;
            //printf("%c", word[len]);
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

    hashtable_t *hashtable = hashtable_create(TABLE_SIZE);
    char check = '';
    while (!feof(fp)) {
        if (check == '\n') {
            break;
        }
        char *word1 = malloc(STR_SIZE);
        read_word(fp, word1, STR_SIZE);
        char *word2 = malloc(STR_SIZE);
        read_word(fp, word2, STR_SIZE);
        char *buffer = malloc(STR_SIZE);

        snprintf(buffer, STR_SIZE * 2, "%s %s", word1, word2);
        printf("%s\n", buffer);
        printf("%ld\n", strlen(buffer));
        hashtable_get(hashtable, &key, &value);
        hashtable_set(hashtable, buffer, value);
    }

    // for (int i = 0; i < 10; i++) {
    //     printf("%c", word1[i]);
    // }
    // for (int i = 0; i < 10; i++) {
    //     printf("%c", word2[i]);
    // }


    // int value = 10;
    // char *key;
    // strcpy(key, "abc");
    // hashtable_set(hashtable, key, value);
    //hashtable_get(hashtable, &key, &value);

    // int bigram_size = hashtable_size();
    // printf("Rehashing reduced collisions from XX to XX");
    // printf("Bigram 'of the' has count of XXX");
    // printf("Total of %d different bigrams recorded", bigram_size);

    hashtable_destroy(hashtable, true); //true = destroy hashtable_t
    free(buffer);
    free(word2);
    free(word1);
    fclose(fp);
    return 0;
}
