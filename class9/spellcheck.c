#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define WORD_MAX_LEN 256

typedef struct tst_node {
    char c;
    // just in this structure definition we have to call the type
    // "struct tst_node" because the tst_node_t definition doesn't exist yet
    struct tst_node *low;
    struct tst_node *equal;
    struct tst_node *high;
} tst_node_t;

typedef struct tst {
    tst_node_t *node;
} tst_t;

tst_node_t *tst_node_create(char c) {
    tst_node_t *node = calloc(1, sizeof(*node)); // calloc also zeros out the memory
    node->c = c;
    node->low = NULL;
    node->high = NULL;
    return node;
}

void tst_node_destroy(tst_node_t *node) {
    if (node) {
        tst_node_destroy(node->low);
        tst_node_destroy(node->high);
        tst_node_destroy(node->equal);
        free(node);
    }
}

tst_t *tst_create(void) {
    return calloc(1, sizeof(tst_t)); // calloc also zeros out the memory
}

void tst_destroy(tst_t *tst) {
    tst_node_destroy(tst->node);
    free(tst);
}

// void tst_add(tst_t *tst, const char *word) {
//     if (!tst->node) {
//         tst->node = tst_node_create(word[0]);
//     }
//     tst_node_t *node = tst->node;
//     while(1) {
//         char c = node->c;
//         if (word[0] < c) {
//             if (!node->low) {
//                 node->low = tst_node_create(word[0]);
//                 //node = node->low;
//             } else {
//                 node = node->low;
//             }
//             word++;
//         } else if (word[0] > c) {
//             if (!node->high) {
//                 node->high = tst_node_create(word[0]);
//                 //node = node->high;
//             } else {
//                 node = node->high;
//             }
//             word++;
//         } else if (word[0] == c) {
//             word++;
//             if (!node->equal) {
//                 node->equal = tst_node_create(word[0]);
//             } else {
//                 node = node->equal;
//             }
//             if (word[0] == '\0' && node->equal == NULL) {
//                 break;
//             }
//         }
//     }
// }

void tst_add(tst_t *tst, const char *word) {
    if (!tst->node) {
        tst->node = tst_node_create(word[0]);
    }
    tst_node_t *node = tst->node;
    while (1) {
        char c = node->c;
        if (word[0] < c) {
            if (!node->low) {
                node->low = tst_node_create(word[0]);
                node = node->low;
                if (word[0] == '\0') {
                    break;
                }
            } else {
                node = node->low;
            }
        } else if (word[0] > c) {
            if (!node->high) {
                node->high = tst_node_create(word[0]);
                node = node->high;
                if (word[0] == '\0') {
                    break;
                }
            } else {
                node = node->high;
            }
        } else if (word[0] == c) {
            if(!node->equal) {
                if (word[0] == '\0') {
                    break;
                } else {
                    node->equal = tst_node_create(word[1]);
                    node = node->equal;
                    word++;
                }
            } else {
                node = node->equal;
                word++;
            }
        }
    }
}

void tst_node_search(tst_node_t *node, char *word, char *suggestion,
                     char *sugg_start, int errs) {
    // while (node) {
    //     * what would happen if we skipped a character in word?
    //         * (to fix insertion errors)
    //         * recursive call to tst_node_search with errs = errs - 1
    //         tst_node_search(node, word, suggestion, sugg_start, errs - 1);
    //     * what would happen if we assumed word has the right character?
    //         * (to fix deletion errors)
    //         * recursive call to tst_node_search with errs = errs - 1
    //         tst_node_search(node, word, suggestion, sugg_start, errs - 1);
    //     * what would happen if we replaced the wrong character in word with the right one?
    //         * (to fix replacement errors)
    //         * recursive call to tst_node_search with errs = errs - 1
    //         tst_node_search(node, word, suggestion, sugg_start, errs - 1);
    //     * what would happen if we swapped the next two characters in word?
    //         * (to fix transposition errors)
    //         * recursive call to tst_node_search with errs = errs - 1
    //         tst_node_search(node, word, suggestion, sugg_start, errs - 1);
        //Actual search
        char c = node->c;
        if (word[0] < c) {
            return tst_node_search(node->low, word, suggestion, sugg_start, 1);
        } else if (word[0] > c) {
            return tst_node_search(node->high, word, suggestion, sugg_start, 1);
        } else {
            if (word[0] == '\0') {
                suggestion[0] = '\0';
                printf("%s", sugg_start);
                return;
            }
            suggestion[0] = c;
            suggestion++;
            word++;
            return tst_node_search(node->equal, word, suggestion, sugg_start, 1);
        }
    }

    // depending on word[0] {
    //     * move to node->left
    //         * what would happen if we went to node->right instead?
    //             * recursive call to tst_node_search with same number of errs
    //     * move to node->right
    //         * what would happen if we went to node->left instead?
    //             * recursive call to tst_node_search with same number of errs
    //     * move to node->equal, advancing both word and suggestion after setting suggestion[0]
    //         * and print out the full suggestion if we matched a null character
    //         * and what if we had gone to node->left or node->right instead?
    //             * recursive calls to tst_node_search with same number of errs
    //             * perform these recursive calls first
    // }
}
}

void tst_search(tst_t *tst, char *word) {
    if (!tst->node) {
        return;
    }
    char word_suggestion[WORD_MAX_LEN];
    tst_node_search(tst->node, word, &word_suggestion[0], word_suggestion, 1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <word>+\n", argv[0]);
        return 1;
    }

    FILE *f = fopen("words.txt", "r");
    if (!f) {
        perror("Failed to open words.txt");
        return 1;
    }

    tst_t *tst = tst_create();

    char str_buffer[WORD_MAX_LEN];
    while (1) {
        int ret = fscanf(f, "%255s", str_buffer);
        if (ret != 1) {
            break;
        }
        // tst_add(tst, str_buffer);
    }
    fclose(f);

    tst_add(tst, "the");
    tst_add(tst, "tea");
    tst_add(tst, "that");
    tst_add(tst, "thee");
    tst_add(tst, "hospital");

    for (int i = 1; i < argc; i++) {
        if (strlen(argv[i]) >= WORD_MAX_LEN) {
            fprintf(stderr, "word '%s' is too long!\n", argv[i]);
            continue;
        }
        tst_search(tst, argv[i]);
        printf("\n");
    }

    tst_destroy(tst);
    return 0;
}
