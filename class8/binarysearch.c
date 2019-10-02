#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void search(const int *data, int n, int input) {
    int leftval = 0;
    int rightval = n;
    while (leftval < rightval) {
        int mid = floor((leftval + rightval) / 2.0);
        if (data[mid] < input) {
            leftval = mid + 1;
        } else {
            rightval = mid;
        }
        printf("mid_i: %d\n", mid);
    }
    if (data[leftval] == input) {
        printf("%d\n", leftval);
    }
}

int int_compare(const void *a, const void *b) {
    int val_a = *(int *)a;
    int val_b = *(int *)b;
    return val_a - val_b;
}

int main(int argc, char **argv) {
    if (argc != 2) { // Print error message if too many arguments
        printf("Wrong num of arguments\n");
        return 1;
    }
    int input = atoi(argv[1]);
    int n = 1000;
    int *data = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        data[i] = rand() % n;
    }
    qsort(data, n, sizeof(int), int_compare);

    search(data, n, input);
    free(data);
    return 0;
}
