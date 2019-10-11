#include <stdio.h>
#include <math.h>

int main(void) {
    printf("Enter a number to factorize:\n"); //read user input
    int number = 0;
    int vals_read = scanf("%d", &number);
    if (vals_read != 1) {
        fprintf(stderr, "Invalid input. Please enter a number.\n");
        return 1;
    }
    printf("Prime factors:\n");
    while (number % 2 == 0) { //remove divisibility by 2
        printf("%d\n", 2);
        number = number / 2;
    } for (int i = 3; i <= sqrt(number); i = i + 2) { //break down the now odd numbers
        while (number % i == 0) {
            printf("%d\n", i);
            number = number / i;
        }
    } if (number > 2) { //identify prime numbers
            printf("%d\n", number);
        }
}

