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
        v->data = realloc(v->data, sizeof(int *) * (2 * v->capacity));
        v->capacity *= 2;
    }
    v->data[v->size] = value;
    v->size++;
}

void vector_free(vector_t *v) {
    free(v->data);
}

//MAIN FUNCTION
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Error: Wrong number of arguments\n");
        return 1;
    }

    long max;
    max = strtol(argv[1], NULL, 10);

    vector_t v;
    vector_create(&v);

    if (max != 0 && max != 1) {
        vector_append(&v, 1);
        vector_append(&v, 1);
        vector_append(&v, 2);
        vector_append(&v, 2);

        for (int i = 3; i <= max; i++) {
            int count = v.data[i];
            for (int j = 0; j < count; j++) {
                vector_append(&v, i);
            }
        }

        for (int i = v.size - 1; i > 0; i--) {
            printf("%d\n", v.data[i]);
        }

    } else if (max == 1) {
        vector_append(&v, 1);
        printf("%d\n", v.data[0]);
    }

    vector_free(&v); //Free allocated memory
    return 0;
}
