#include <stdio.h>

int main(void) {
    int i = 0;
    while (i<100){
        if (i%15==0)
        {
            printf("fizzbuzz\n");
        }
        if (i%5==0)
        {
            printf("fizz\n");
        }
        else if (i%3==0)
        {
            printf("buzz\n");
        }
        else
        {
            printf("%d\n",i);
        }
        i++;
    }
    return 0;
}
