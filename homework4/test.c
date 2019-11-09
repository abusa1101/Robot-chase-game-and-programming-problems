#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

int main(void) {
    char *p1 = "age";
    char *p2 = "siblings";
    char *p3 = "height";
    char *p4 = "weight";
    char *p5 = "apples";
    char *p6 = "oranges";

    int pval = 0;
    int *pval_ptr = &pval;

    hashtable_t *student = hashtable_create(4);

    hashtable_set(student, p1, 6);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    //hashtable_set(student, p2, 270);
    //printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    // hashtable_set(student, p3, 90);
    // printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    // hashtable_set(student, p4, 30);
    // printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    // hashtable_set(student, p5, 30);
    // printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    // hashtable_set(student, p6, 1005);
    // printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));

    // hashtable_get(student, p6, pval_ptr);
    // printf("ptr: %d\n", *pval_ptr);

    hashtable_destroy(student, true);
    return 0;
}
