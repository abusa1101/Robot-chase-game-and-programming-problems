#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {

//unsigned char text;

unsigned char text = argv[1][0];
//printf("%d\n", argc);
//printf("%c\n", text);

// Print 0 if no input is given
if (argc == 1) { 
    printf("0");
}

// Print error message if too many arguments
if (argc > 2) {
    printf("Too many arguments\n");
    return 1;
}

int len = strlen(argv[1]);
//printf("%d\n",len);

int counter = 0;
int totalcount = 0;

for (int i = 0; i < len; i++) { //i is which letter of the word you are on
    text = argv[1][i];
    //printf("%c",text);
    //totalcount = totalcount + counter;
    for(counter = 0; text!= 0; text >>= 1) { //for each letter
        if(text & 01) {
            counter++; //counts the number of ones in the word's binary form
            //printf("%d\n",counter); 
            totalcount = totalcount + 1;           
        }
    }
}
//printf("%d\n",totalcount);

if (totalcount % 2 == 0) {
    printf("0\n");
} else {
    printf("1\n");
}
    return 0;
}

/*  int b = 0;
    while (x != 0) {
        if (x & 0x1)
            b++;
        x = x >> 1;
    }

    return b;
    */



   /*for (int j = 1; j < argc; j++) { //j is which word are you on

       int len = strlen(argv[j]);

    //convert char to ASCII

    //convert ASCII to binary

    //count the 1's in the binary- if count is even print 0, else print 1
    int binarynum = 0; //will change
    int counter = 0;
    int binlen = strlen(binarynum);
    for (int i = 0; i < binlen; i++) {
        if (i == 1){
            counter = counter + 1;
        }
    }
    if (counter % 2 == 0){
        printf("0");
    } else {
        printf("1");
    }*/