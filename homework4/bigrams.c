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
#define STR_SIZE 512

//BIGRAM FUNCTIONS
// void skip(FILE *fp) {
//     while (!feof(fp) && !isalpha(fgetc(fp))) {
//     }
//     // if (!feof(fp)) {
//     //     fseek(fp, -1, SEEK_CUR);
//     // }
// }

void read_word(FILE *fp, char *word, int char_num) {
    // int len = 0;
    // while (1) {
    //     char c = fgetc(fp);
    //     if (isalpha(c)) {
    //         word[len] = c;
    //         len++;
    //     } else {
    //         //c = fgetc(fp);
    //         // word[len] = ' ';
    //         // len++;
    //         word[len] = ' ';
    //         len++;
    //         // return;
    //     }
    //     if (feof(fp) || c == ' ' || word[len] == ' ') {
    //         word[len] = '\0';
    //         return;
    //     }
    //     //skip(fp);
    // }
    char c = fgetc(fp);
    while (!isalpha(c)) {
        c = fgetc(fp);
        if (feof(fp)) {
            break;
        }
    }
    while (isalpha(c)) {
        *word = c;
        word++;
        c = fgetc(fp);
    }
    *word = '\0';
}

int main(void) {
    FILE *fp = fopen("book.txt", "r");
    if (!fp) {
        fprintf(stderr, "Error: Missing file.\n");
        exit(1);
    }
    int value = 0;
    char *word = malloc(STR_SIZE);
    read_word(fp, word, STR_SIZE);
    hashtable_t *hashtable = hashtable_create(TABLE_SIZE);
    while (!feof(fp)) {
        char *prev_word = word;
        word = malloc(STR_SIZE);
        read_word(fp, word, STR_SIZE);
        char *buffer = malloc(STR_SIZE);
        //if (word) {
            snprintf(buffer, STR_SIZE, "%s %s", prev_word, word);
        //}
        //printf("%s\n", buffer);

        if (hashtable_get(hashtable, buffer, &value)) {
            hashtable_set(hashtable, buffer, value + 1);
        } else {
            hashtable_set(hashtable, buffer, 1);
        }

        free(buffer);
        free(prev_word);
    }
    free(word);

    int bigram_size = hashtable_size(hashtable);
    bool is200bigrams = false;
    int n = hashtable_probe_max(hashtable);


    for (int i = 0; i < n; i++) {
        char *key;
        int val;
        if (hashtable_probe(hashtable, i, &key, &val)) {
            if (val >= 200) {
                is200bigrams = true;
                printf("Bigram '%s' has count of %d\n", key, val);
            }
        }
    }


    if(!is200bigrams) {
        char *key2;
        for (int i = 0; i < n; i++) {
            int val2;
            if (hashtable_probe(hashtable, i, &key2, &val2)) {
                printf("Bigram '%s' has count of %d\n", key2, val2);
            }
        }
    }
    printf("Total of %d different bigrams recorded\n", bigram_size);

    hashtable_destroy(hashtable, true); //true = destroy hashtable_t
    fclose(fp);
    return 0;
}
