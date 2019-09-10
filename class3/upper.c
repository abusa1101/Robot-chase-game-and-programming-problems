#include <stdio.h>
#include <math.h>

int main(void) {
    //printf("Enter text to capitalize:\n"); //read user input
    char word;
    int vals_read = scanf("%s", word); //or whatever input
    len = strlen(word);

    for (int i = o; i <= len; i++){
        if (word(i) >= 97 && word(i) <= 122){
            word(i)= word(i) - 32;
            letter = word(i);
            printf("%s", letter);
        }
        else {
            printf("Enter lowercase letters between 'a' and 'z' only.\n");
            return 1;
        }
    }



    /*if (vals_read = ) {
        fprintf(stderr, "Invalid input. Please enter a word.\n");
        return 1;
    }*/
    
}

