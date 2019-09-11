#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    unsigned char text;
    if (argc == 1) { // Print 0 if no input is given
        printf("0");
    }

    if (argc > 2) { // Print error message if too many arguments
        printf("Too many arguments\n");
        return 1;
    }

    int len = strlen(argv[1]);
    int counter = 0;
    int totalcount = 0;
    for (int i = 0; i < len; i++) { //i is which letter of the word you are on
        text = argv[1][i];
        for (counter = 0; text != 0; text >>= 1) { //for each letter
            if (text & 01) {
                counter++; //counts the number of ones in the word's binary form
                totalcount = totalcount + 1;
            }
        }
    }

    if (totalcount % 2 == 0) {
        printf("0\n");
    } else {
        printf("1\n");
    }
    return 0;
}
