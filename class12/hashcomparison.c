#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define TABLE_SIZE 8192

typedef struct test_entry {
    uint8_t *data;
    int n;
} test_entry_t;

void evaluate_hash_reduce(int n_entries, test_entry_t *entries, bool fast_mode,
                          uint32_t (*hash_f)(uint8_t *, int), uint32_t (*reduce_f)(uint32_t)) {
    clock_t start = clock();
    uint32_t hash = reduce_f(hash_f(entries[0].data, entries[0].n));
}

int main(int argc, char **argv) {
    // First we need to collect all the entries/strings/data that we will try to hash
    // This needs to be done up front for the benchmarking later to be valid.
    int max_entries = TABLE_SIZE / 2;
    test_entry_t *entries = calloc(max_entries, sizeof(test_entry_t));

    // This is an array of function pointers
    // Which will let us iterate through all the different hash functions
    // The syntax is actually only slightly different from a normal function
    // for a function pointer, the name of the function is in parenthesis
    // and it has an asterisk in front of it. The brackets after the name
    // show it is an array of function pointers. Everything outside those parentheses
    // work like normal for the function definition, and the arguments don't need names.
    uint32_t (*hash_functions[])(uint8_t *, int) = {add_hash, table_a_hash, table_b_hash,
                                                    djb2a_hash, fnv1a_hash, fxhash32_hash};
    // This is a good idiom to get the number of elements in a static array in C
    // This only works because the hash function array is declared as a literal array with { ... }
    int n_hash_functions = sizeof(hash_functions) / sizeof(hash_functions[0]);

    uint32_t (*reduce_functions[])(uint32_t) = {modulo2_reduce, modulo_prime_reduce,
                                                fibonacci32_reduce};
    int n_reduce_functions = sizeof(reduce_functions) / sizeof(reduce_functions[0]);

    for (int hash_i = 0; hash_i < n_hash_functions; hash_i++) {
        for (int reduce_i = 0; reduce_i < n_reduce_functions; reduce_i++) {
            evaluate_hash_reduce(n_entries, entries, fast_mode, hash_functions[hash_i],
                                 reduce_functions[reduce_i]);
        }
        printf("\n");
    }

    return 0;
}
