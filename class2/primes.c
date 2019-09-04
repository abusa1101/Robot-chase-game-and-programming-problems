#include <stdio.h>
#include <math.h>

int main(void) {
    //read user input
    printf("Enter a number to factorize:\n");
    int number = 0;
    int vals_read = scanf("%d", &number);

    if (vals_read != 1) {
        fprintf(stderr, "Invalid input. Please enter a number.\n");
        return 1;
    }

    //remove divisibility by 2
    while (number % 2 == 0) {
        printf("%d", 2);
        number = number / 2;
    } 

    //break down the now odd numbers
    for (int i = 3; i <= sqrt(number); i=i+2){
       // printf("in the for loop\n");

        while (number % i == 0){
            //printf("in the while loop\n");
            printf("%d", i);
            number = number/i;
        }
    }

        //identify prime numbers
        if (number > 2){
            //printf("in the if statement\n");
            printf("%d",number);
        }
        
    
}
