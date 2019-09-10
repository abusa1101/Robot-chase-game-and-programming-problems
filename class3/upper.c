#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

    char letter;
    letter = argv[1][0];
    int k = 0;
    //int len2 = strlen(argv[2]);
    for (int j = 1; j < argc; j++){ //j is which word are you on
        int len = strlen(argv[j]);
        for (int i = 0; i < len; i++){ //i is which letter of the word you are on
            letter = argv[j][i];
            if (letter >= 96 & letter <= 122){
                letter = (letter - 32);
                printf("%c", letter);
            }

            else if (letter == 34){
                k = 1;
                letter = argv[j][i+1];
                if (k == 1){
                    printf("\n");
                }
            
            }

            else {
                printf("%c", letter);
                
            }
        }
        k = 0;
    }
    printf("\n"); 
}


