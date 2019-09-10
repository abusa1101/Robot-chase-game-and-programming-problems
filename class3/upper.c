#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Please enter text");
        return 1;
    }
    char letter;
    for (int j = 1; j < argc; j++) { //j is which word are you on
        int len = strlen(argv[j]);
        for (int i = 0; i < len; i++) { //i is which letter of the word you are on
            letter = argv[j][i];
            if (letter >= 96 & letter <= 122) {
                letter = (letter - 32);
                printf("%c", letter);
            } else {
                printf("%c", letter);
            }
        }
    }
    printf("\n");
    return 0;
}
