#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

__uint64_t seq[100] = {0, 1, 1, 0};

__uint64_t sequence(__uint64_t n) {
    if (n < 1) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }
    n = sequence(n - 1) + sequence(n - 2);
    return n;
}

__uint64_t sequence2(__uint64_t n) {
    if (n <= 1) {
        return 1;
    }
    __uint64_t fib = 0;
    __uint64_t fib2 = 0;
    __uint64_t fib1 = 1;
    for (int i = 2; i <= n; i++) {
        fib = fib1 + fib2;
        fib2 = fib1;
        fib1 = fib;
        seq[i] = fib;
    }
    return fib;
}

__uint64_t sequence3(__uint64_t n) {
    if (seq[n]) {
        return seq[n];
    }
    seq[n] = sequence3(n - 1) + sequence3(n - 2);
    return seq[n];
}

int main(void) {
    clock_t start = clock();
    __uint64_t rs = sequence(38);
    double elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    printf("fib1(38) got %ld and took %.6f ms per iteration\n", rs, elapsed * 100);

    clock_t start2 = clock();
    __uint64_t ls = sequence2(90);
    double elapsed2 = (clock() - start2) / (double)CLOCKS_PER_SEC;
    printf("fib2(90) got %ld and took %.6f ms per iteration\n", ls, elapsed2 * 100);

    clock_t start3 = clock();
    __uint64_t ms = sequence3(90);
    double elapsed3 = (clock() - start3) / (double)CLOCKS_PER_SEC;
    printf("fib3(90) got %ld and took %.6f ms per iteration\n", ms, elapsed3 * 1000);

    return 0;
}

//int i = 1;
// int n = 2;
// int fn;
// //fn = sequence2(18);
// double elapsed = 0;
// while (elapsed < 0.49) {
//   clock_t start = clock();
//   //fn = sequence(i);
//   fn = sequence2(n);
//   elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
//   n = fn;
// }
// printf("took %.2f seconds\n", elapsed);
// //printf("n: %d\n", fn);
// printf("n: %d\n", fn);
// //printf("i: %d\n", i);
