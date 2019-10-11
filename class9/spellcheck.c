#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define WORD_MAX_LEN 256

// ternary search tree node
typedef struct tst_node {
    char c;
    // just in this structure definition we have to call the type
    // "struct tst_node" because the tst_node_t definition doesn't exist yet
    struct tst_node *low;
    struct tst_node *equal;
    struct tst_node *high;
} tst_node_t;

// ternary search tree
typedef struct tst {
    tst_node_t *node;
} tst_t;

tst_node_t *tst_node_create(char c) {
    tst_node_t *node = calloc(1, sizeof(*node)); // calloc also zeros out the memory
    node->c = c;
    return node;
}

void tst_node_destroy(tst_node_t *node) {

}

tst_t *tst_create(void) {
    return calloc(1, sizeof(tst_t)); // calloc also zeros out the memory
}

void tst_destroy(tst_t *tst) {

}

void tst_add(tst_t *tst, const char *word) {
    make root node of tree if it doesn't exist
    forever {
        depending on word[0] {
            * move to node->left, creating it if it doesn't exist
            * move to node->right, creating it if it doesn't exist
            * move to node->equal, returning if word[0] is '\0'
                * advancing word (moving the pointer with word++) if word[0] is not '\0',
                * creating node->equal if it doesn't exist
        }
    }
}

void tst_node_search(tst_node_t *node, char *word, char *suggestion, char *sugg_start, int errs) {

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
        tst_add(tst, str_buffer);
    }
    fclose(f);

    // tst_add(tst, "the");
    // tst_add(tst, "tea");
    // tst_add(tst, "that");
    // tst_add(tst, "thee");
    // tst_add(tst, "hospital");

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
