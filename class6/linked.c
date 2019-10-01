#include <stdio.h>
#include <stdlib.h>
#include <string.h>

<<<<<<< HEAD
// A linked list node
struct Node {
    int value;
    struct Node* next;
    struct Node* prev;
};
=======
typedef struct node {
    struct node *prev;
    struct node *next;
    int value;
} node_t;
>>>>>>> 34afee1851efd6c26699b45918b1eb0331429daa

typedef struct list {
    node_t *start;
    node_t *end;
} list_t;

<<<<<<< HEAD
list_t *list_create(void) {
  //set list to null to indicate empty list
  /* Start with the empty list */
  list_t *list = NULL;
  // list_t list;
  // list.start = (int*) calloc(1, sizeof(int));
  // list.end = (int*) calloc(1, sizeof(int));
}

void list_push_start(list_t *list, int value) {
  /*//at this point DLL is NOT empty
  int *head_ref = &list;

  node_t first_node = (node_t)malloc(sizeof(node_t)); //create first node
  new_node->value = new_data; //inserting node value
  //check if DLL is empty, head NULL or size 0
  if (*head_ref == NULL) {}
  //if yes, set prev as null and next to head pointer
  //because its first node
  new_node->next = (head_ptr); //make next of new node as head of list
  new_node->prev = NULL; //make previous of new node as NULL
  if ((*head_ref) != NULL) //change prev of head node to new node
      (*head_ref)->prev = new_node;
  (*head_ref) = new_node; //head now points to new node
  //if no, set it as front node--- not sure about below code
  new_node->next = (head_ptr); //make next of new node as head
  if ((*head_ref) != NULL) //change prev of head node to new node
      (*head_ref)->prev = new_node;
  (*head_ref) = new_node; //head now points to new node

  /* 1. allocate node */
  struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));

  /* 2. put in the data  */
  new_node->data = new_data;

  /* 3. Make next of new node as head and previous as NULL */
  new_node->next = (*head_ref);
  new_node->prev = NULL;

  /* 4. change prev of head node to new node */
  if ((*head_ref) != NULL)
      (*head_ref)->prev = new_node;

  /* 5. move the head to point to the new node */
  (*head_ref) = new_node;

}

void list_push_end(list_t *list, int value) {
  struct Node* new_node = (struct Node*)malloc(sizeof(struct Node)); //allocating the new last node
  new_node->data = new_data; //insert value at new last node
  new_node->next = NULL; // this is last node so make next node pointer NULL

  if (*head_ref == NULL) { //if DLL is empty, make this the head node
      new_node->prev = NULL;
      *head_ref = new_node;
      return;
  }
  struct Node* last = *head_ref;
  while (last->next != NULL) //find the current last node
      last = last->next;

  last->next = new_node; //set next of existing last node to new last node
  new_node->prev = last; //set current last node as previous of new last node

  return;
}

int list_pop_start(list_t *list) {
}

int list_pop_end(list_t *list) {
}

void list_destroy(list_t *list) {
  //call pop function until list is all gone, that is all values == NULL
}
=======
list_t *list_create(void);
void list_push_start(list_t *list, int value);
void list_push_end(list_t *list, int value);
int list_pop_start(list_t *list);
int list_pop_end(list_t *list);
void list_destroy(list_t *list);
>>>>>>> 34afee1851efd6c26699b45918b1eb0331429daa

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
<<<<<<< HEAD

    fclose(f);

    return 0;
}
=======
    fclose(f);
    return 0;
}

//DLL Functions
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
>>>>>>> 34afee1851efd6c26699b45918b1eb0331429daa
