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

    //printf("%d\n",number);
    //int number = 18;

    //calculate prime factors
    if (number == 1){
        printf("%d\n", number);
    }
    //remove divisibility by 2
    while (number % 2 == 0){
        printf("%d\n",number);
        number = number/2;
        //printf("%d\n",number);
        //if (number %2 != 0){
            //break;
        //}
    } 
    //printf("%d\n", number);

    
    /*
    int i = 1;
    //break down odd numbers
    for (i < sqrt(number)){
        while (number % i == 0){
            printf("%d", i);
            number = number/i;
        }

        //identify prime numbers
        if (number > 2){
            printf("d",number);
        }
        
        //i = i + 2;}*/
}

