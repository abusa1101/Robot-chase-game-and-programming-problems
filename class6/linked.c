#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node *prev;
    struct node *next;
    int value;
} node_t;

typedef struct list {
    node_t *start;
    node_t *end;
} list_t;

list_t *list_create(void);
void list_push_start(list_t *list, int value);
void list_push_end(list_t *list, int value);
int list_pop_start(list_t *list);
int list_pop_end(list_t *list);
void list_destroy(list_t *list);

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "could not open %s: ", argv[1]);
        perror("");
        return 1;
    }

    list_t *list = list_create();

    while (!feof(f)) {
        char command[16];
        int res = fscanf(f, "%10s", command);
        if (res != 1) {
            if (!feof(f)) {
                fprintf(stderr, "trouble parsing command. stopping.\n");
            }
            break;
        }

        if (strcmp("push_start", command) == 0) {
            int value = 0;
            res = fscanf(f, "%d", &value);
            if (res != 1) {
                fprintf(stderr, "trouble parsing value. stopping.\n");
                break;
            }

            list_push_start(list, value);
        } else if (strcmp("push_end", command) == 0) {
            int value = 0;
            res = fscanf(f, "%d", &value);
            if (res != 1) {
                fprintf(stderr, "trouble parsing value. stopping.\n");
                break;
            }

            list_push_end(list, value);
        } else if (strcmp("pop_start", command) == 0) {
            int v = list_pop_start(list);
            printf("%d\n", v);
        } else if (strcmp("pop_end", command) == 0) {
            int v = list_pop_end(list);
            printf("%d\n", v);
        } else {
            fprintf(stderr, "did not recognize command '%s'. stopping.\n", command);
            break;
        }
    }

    list_destroy(list);
    fclose(f);
    return 0;
}

list_t *list_create(void) {
    list_t *list = malloc(sizeof(list_t));
    list->start = NULL;
    list->end = NULL;
    return list;
}

void list_push_start(list_t *list, int value) {
    node_t *node = malloc(sizeof(node_t)); //create node
    if (!list->end) {
        node->value = value; //set node value
        node->next = NULL;   //set node pointers to NULL
        node->prev = NULL;
        list->end = node;
        list->start = node;
    } else {
        node->value = value;
        list->start->prev = node;
        node->next = list->start;
        list->start = node;
        node->prev = NULL;
    }
}

void list_push_end(list_t *list, int value) {
    node_t *node = malloc(sizeof(node_t));
    if (!list->end) {
        node->value = value;
        node->next = NULL;
        node->prev = NULL;
        list->start = node;
        list->end = node;
    } else {
        node->value = value;
        list->end->next = node;
        node->prev = list->end;
        list->end = node;
        node->next = NULL;
    }
}

int list_pop_start(list_t *list) {
    if (!list->start) {
        fprintf(stderr, "Error: List is empty\n");
        exit(1);
    }
    int value = list->start->value;
    node_t *start_node = list->start;
    if (!start_node->next) {
        list->start = NULL;
        list->end = NULL;
    } else {
        list->start = start_node->next;
        list->start->prev = NULL;
    }
    free(start_node);
    return value;
}

int list_pop_end(list_t *list) {
    if (!list->end) {
        fprintf(stderr, "Error: List is empty\n");
        exit(1);
    }
    int value = list->end->value;
    node_t *end_node = list->end;
    if (!end_node->prev) {
        list->end = NULL;
        list->start = NULL;
    } else {
        list->end = end_node->prev;
        list->end->next = NULL;
    }
    free(end_node);
    return value;
}

void list_destroy(list_t *list) {
    while (list->end) {
        list_pop_end(list);
    }
    free(list);
}
