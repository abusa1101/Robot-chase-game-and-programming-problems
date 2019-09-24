#include <stdio.h>
#include <stdlib.h>

//vector structure
typedef struct vector {
    int *data; //pointer points to address of the first element of the array
    int capacity;
    int size;
} vector_t;

//functions
void vector_create(vector_t *v) {
    v->capacity = 2;
    v->size = 0;
    v->data = malloc(sizeof(int *) * v->capacity);
}

void vector_append(vector_t *v, int value) {
  if (v->capacity == v->size) {
    //v -> data = realloc(v-> data, sizeof(* v -> data) * (2 * v -> capacity));
    v->data = realloc(v->data, sizeof(int *) * (2 * v->capacity));
    v->capacity *= 2;
  }
  v->data[v->size] = value;
  v->size++;
}

void vector_free(vector_t *v) {
  free(v->data);
}

int golomb_alg(vector_t *v, int input) {
  if (input == 1) {
    return 1;
    vector_append(v, 1);
  }
  int result = 1 + golomb_alg(v, input - golomb_alg(v, golomb_alg(v, input - 1)));
  return result;
}

void golomb_find(vector_t *v, int input) {
  int i = 1;
  while (golomb_alg(v, i) != (input) + 1) {
    vector_append(v, golomb_alg(v, i));
    i++;
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Error: Wrong number of arguments\n");
    return 1;
  }

  long max;
  max = strtol(argv[1], NULL, 10);

  vector_t v;
  vector_create(&v);
  golomb_find(&v, max);
  for (int i = v.size - 1; i >= 0; i--) {
    printf("%d\n", v.data[i]);
  }

  vector_free(&v);
  v.data = NULL;
  return 0;
}
