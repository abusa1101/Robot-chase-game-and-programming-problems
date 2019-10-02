#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

int sequence(int n) {
  if (n <=1) {
    return 1;
  }
  n = sequence(n-1) + sequence(n-2);
  return n;
}

int sequence2(int n) {
  if (n <=1) {
       return 1;
     }
     int fib = 0;
     int fib2 = 0;
     int fib1 = 1;
  for (int i = 2; i <= n; i++) {
    fib = fib1 + fib2;
    fib2 = fib1;
    fib1 = fib;
  }
  return fib;
}

int main(void) {

    clock_t start = clock();
    int rs = sequence(38);
    double elapsed = (clock() - start) / (double)CLOCKS_PER_SEC;
    printf("fib1(38) got %d and took %.2f ms per iteration\n", rs, elapsed*1000);

    clock_t start2 = clock();
    int ls = sequence2(90);
    double elapsed2 = (clock() - start2) / (double)CLOCKS_PER_SEC;
    printf("fib2(90) got %d and took %.2f ms per iteration\n", ls, elapsed*1000);

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
