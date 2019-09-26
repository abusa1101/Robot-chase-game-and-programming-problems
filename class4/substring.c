
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char file_buffer[64 * 1024];

char *print_context(char *p, int count, int key_len) {
    if (count == 0) {
        int m = 0;
        while (m < key_len) {
            printf("%c", p[m]);
            m++;
        }
        printf("\n");
        printf("\n");
        return &p[m];
    }
    int i = 0;
    for (int n = 0; n < count; n++) {
        while (p[-i] != '\n' && &p[-i] >= file_buffer) {
            i++;
        }
        if (&p[-i] >= file_buffer) {
            i++;
        }
    }
    int j;
    if (&p[-i] <= file_buffer) {
        j = -i + 1;
    } else {
        j = -i + 2;
    }
    while (j < key_len) {
        printf("%c", p[j]);
        j++;
    }
    printf("\n");
    printf("\n");
    return &p[j];
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "usage: %s <file> <key> <lines before>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    int text = atoi(argv[3]);
    int key_len = strlen(argv[2]);
    if (!f) {
        fprintf(stderr, "Could not open %s: ", argv[1]);
        perror("");
        return 1;
    }
    fread(file_buffer, 1, sizeof(file_buffer) - 1, f);
    if (!feof(f)) {
        fprintf(stderr, "Could not read entire file. Is it too big?\n");
        return 1;
    }
    char *p = strstr(file_buffer, argv[2]);
    while (p) {
        char *p = print_context(p, text, key_len);
        p = strstr(p, argv[2]);
    }
    fclose(f);
    return 0;
}
