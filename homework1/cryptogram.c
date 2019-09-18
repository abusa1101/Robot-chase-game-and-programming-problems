#include <stdio.h>
#include <string.h>

int chartonum(char h) {
    if (h == '\0') {
        return 300;
    }
    int num;
    if (h >= 'A' && h <= 'Z') {
        num = h - 'A';
    } else if (h >= 'a' && h <= 'z') {
        num = h - 'a';
    } else {
        num = 100;
    }
    return num;
}

int main(int argc, char **argv) {
    int key = 2;
    int text = 3;

    if (argc != 4) {
        fprintf(stderr, "Error: Too little arguments.\n");
        return 1;
    }

    if (argv[key][0] == '\0') {
        printf("%s", argv[text]);
    }

    unsigned char let_key;
    unsigned char let_word;
    int len_word = strlen(argv[text]);
    int len_key = strlen(argv[key]);

    if (strcmp(argv[1], "encrypt") == 0) {
        int i = 0;
        int j = 0;
        int k = 0;
        while (i <= len_word - 1) {
            if (j == len_key) {
                j = 0;
            }
            let_word = argv[text][i];
            let_key = argv[key][j];
            int num_key = chartonum(let_key);
            if (num_key != 300) {
                while (num_key == 100) {
                    j = j + 1;
                    let_key = argv[key][j];
                    num_key = chartonum(let_key);
                }
                if (let_word >= 'A' && let_word <= 'Z') {
                    let_word = let_word + num_key;
                    if (let_word > 26) {
                        let_word = let_word - num_key;
                        let_word = (let_word + num_key - 'A') % 26 + 'A';
                    }
                } else if (let_word >= 'a' && let_word <= 'z') {
                    let_word = let_word + num_key;
                    if (let_word > 122) {
                        let_word = let_word - num_key;
                        let_word = (let_word + num_key - 'a') % 26 + 'a';
                    }
                } else {
                    j = j - 1;
                }
                i++;
                j++;
            } else {
                break;
            }
            printf("%c", let_word);
        }
        printf("\n");
    } else if (strcmp(argv[1], "decrypt") == 0) {
        int i = 0;
        int j = 0;
        while (i <= len_word - 1) {
            if (j == len_key) {
                j = 0;
            }
            let_word = argv[text][i];
            let_key = argv[key][j];
            int num_key = chartonum(let_key);
            if (num_key != 300) {
                while (num_key == 100) {
                    j = j + 1;
                    let_key = argv[key][j];
                    num_key = chartonum(let_key);
                }
                if (let_word >= 'A' && let_word <= 'Z') {
                    let_word = let_word - num_key;
                } else if (let_word >= 'a' && let_word <= 'z') {
                    let_word = let_word - num_key;
                    if (let_word > 122) {
                        let_word = let_word + num_key;
                        let_word = (let_word - num_key + 'a') % 26 - 'a';
                    }
                } else {
                    j = j - 1;
                }
                printf("%c", let_word);
                i++;
                j++;
            } else {
                break;
            }
        }
        printf("\n");
    } else {
        fprintf(stderr, "expected command encrypt or decrypt.\n");
        return 1;
    }

    return 0;
}
