#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char file_buffer[64 * 1024];
char *get_context(char *context_p, int counter, int len); //func declaration

int main(int argc, char **argv) {
    if (argc != 4) { //No of arguments error
        fprintf(stderr, "usage: %s <file> <key> <lines before>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r"); //open and read file

    int len = strlen(argv[2]);
    int text = atoi(argv[3]);

    if (!f) { //file open error
        fprintf(stderr, "Could not open %s: ", argv[1]);
        perror("");
        return 1;
    }

    fread(file_buffer, 1, sizeof(file_buffer) - 1, f); //read file

    if (!feof(f)) { //file read error
        fprintf(stderr, "Could not read entire file. Is it too big?\n");
        return 1;
    }

    char *context_p = strstr(file_buffer, argv[2]); //string comparison for key word and text

    while (context_p) {
        char *p = get_context(context_p, text, len);
        context_p = strstr(p, argv[2]);
    }

    fclose(f);
    return 0;
}

//function
char *get_context(char *context_p, int counter, int len) {
    if (counter == 0) {
        int m = 0;
        while (m < len) {
            printf("%c", context_p[m]);
            m++;
        }
        printf("\n");
        printf("\n");
        return &context_p[m];
    }

    int i = 0;
    for (int n = 0; n < counter; n++) {
        while (context_p[-i] != '\n' && &context_p[-i] >= file_buffer) {
            i++;
        }
        if (&context_p[-i] >= file_buffer) {
            i++;
        }
    }
    int j = 0;
    if (&context_p[-i] <= file_buffer) {
        j = -i + 1;
    } else {
        j = -i + 2;
    }
    while (j < len) {
        printf("%c", context_p[j]);
        j++;
    }
    printf("\n");
    printf("\n");
    return &context_p[j];
}
