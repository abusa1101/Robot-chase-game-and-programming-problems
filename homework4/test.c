#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

int main(void) {
    char *p1 = "agecount";
    char *p2 = "family";
    char *p3 = "heighthduiefjbsfk";
    char *p4 = "uiqwaefhbkafebukjb";
    char *p5 = "uwieaihfbvkbesukkdfbgbkjknbvdugbj";
    char *p6 = "hi";

    int pval = 0;
    int *pval_ptr = &pval;

    hashtable_t *student = hashtable_create(2);

    hashtable_set(student, p1, 40);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    hashtable_set(student, p2, 370);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    hashtable_set(student, p3, 90);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    hashtable_set(student, p4, 20);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    hashtable_set(student, p5, 20);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));
    hashtable_set(student, p6, 2100);
    printf("filled %d out of %d\n", hashtable_size(student), hashtable_probe_max(student));

    // hashtable_get(student, p6, pval_ptr);
    // printf("ptr: %d\n", *pval_ptr);

    hashtable_destroy(student, true);
    return 0;
}
