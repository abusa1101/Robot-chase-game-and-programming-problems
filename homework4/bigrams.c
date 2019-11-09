#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "hashtable.h"
#define STR_SIZE 256

//BIGRAM FUNCTIONS
void read_word(FILE *fp, char *word, int char_num) {
    int len = 0;
    while (1) {
        char c = fgetc(fp);
        if (feof(fp) || c == ' ') {
            word[len] = '\0';
            return;
        }
        if (isalpha(c)) {
            word[len] = c;
            len++;
        }
    }
}

int main(void) {
    FILE *fp = fopen("book2.txt", "r");
    if (!fp) {
        fprintf(stderr, "Error: Missing file.\n");
        exit(1);
    }

    char *word = malloc(STR_SIZE);
    read_word(fp, word, STR_SIZE);
    hashtable_t *hashtable = hashtable_create(TABLE_SIZE);
    while (!feof(fp)) {
        char *prev_word = word;
        word = malloc(STR_SIZE);
        read_word(fp, word, STR_SIZE);
        char *buffer = malloc(STR_SIZE);
        snprintf(buffer, STR_SIZE, "%s %s", prev_word, word);
        printf("%s\n", buffer);
        //printf("\n%ld\n", strlen(buffer));
        //hashtable_get(hashtable, &key, &value);
        //hashtable_set(hashtable, buffer, value);
        free(buffer);
        free(prev_word);
    }
    free(word);

    // int n = hashtable_probe_max(hashtable);
    // for (int i = 0; i < n; i++) {
    //     char *key;
    //     int val;
    //     if (hashtable_probe(hashtable, i, &key, &val)) {
    //         // do something
    //     }
    // }
    // int bigram_size = hashtable_size();
    // printf("Bigram 'of the' has count of XXX");
    // printf("Total of %d different bigrams recorded", bigram_size);

    hashtable_destroy(hashtable, true); //true = destroy hashtable_t
    fclose(fp);
    return 0;
}
