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

void read_word(FILE *fp, char *word, int char_num);
