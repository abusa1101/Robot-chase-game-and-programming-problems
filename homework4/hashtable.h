#pragma once

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

typedef struct hashtable hashtable_t;

hashtable_t *hashtable_create(int table_size);
void hashtable_destroy(hashtable_t *hashtable, bool free_table);
void hashtable_set(hashtable_t *hashtable, char *key, int value);
bool hashtable_get(hashtable_t *hashtable, char *key, int *value);
int hashtable_probe_max(hashtable_t *hashtable);
bool hashtable_probe(hashtable_t *hashtable, int n, char **key, int *value);
// int hashtable_collisions(hashtable_t *hashtable);
int hashtable_size(hashtable_t *hashtable);
// void rehash(hashtable_t *old_hashtable);
void skip(FILE *fp);
void read_word(FILE *fp, char *word, int char_num);
