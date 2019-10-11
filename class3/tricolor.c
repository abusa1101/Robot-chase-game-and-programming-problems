#include <stdio.h>
#include <stdlib.h>

char *trit_encodings[243];
int trinum[243];
char tri[243];

int main(void) {
    trit_encodings[0] = calloc(sizeof(char), 6); //PART 1: LOOKUP TABLE
    int i = 0;
    int val = i;
    trinum[0] = val % 3;
    if (trinum[0] == 0) {
        tri[0] = ' ';
        trit_encodings[0][0] = ' ';
        trit_encodings[0][1] = ' ';
        trit_encodings[0][2] = ' ';
        trit_encodings[0][3] = ' ';
        trit_encodings[0][4] = ' ';
    } //printf("%c\n",trit_encodings[0][0]); //printf("%d",trinum[0]);

    for (i = 1; i < 243; i++) {
        trit_encodings[i] = calloc(sizeof(char), 6);
        int val = i;
        int j = 0;
        trit_encodings[i][1] = ' ';
        trit_encodings[i][2] = ' ';
        trit_encodings[i][3] = ' ';
        trit_encodings[i][4] = ' ';
        while (val > 0) {
            trinum[i] = val % 3;
            val = val / 3; //printf("%d",trinum[i]);
            if (trinum[i] == 0) {
                tri[i] = ' ';
            }
            if (trinum[i] == 1) {
                tri[i] = ':';
            }
            if (trinum[i] == 2) {
                tri[i] = '@';
            } //printf("%c",tri[i]);
            trit_encodings[i][j] = tri[i];
            j = j + 1;
        } //printf("\n");These print statements were placed on the next line
    }

    FILE *f; //PART 2: IMAGE MANIPULATION
    f = fopen("img.bin", "rb");
    for (int row = 0; row < 40; row++) {
        for (int byte = 0; byte < 18; byte++) {
            char c = fgetc(f);
            printf("%s", trit_encodings[(__uint8_t)c]);
        }
        printf("\n");
    }
}
