#include <stdio.h>
#include <string.h>

void encrypt()
{
 
}

void decrypt()
{
    printf("NOOOOOO\n");
}

int chartonum(char h) //int num_key should be 0,1,2 for a,b,c respectively
{ 
    int num;
    if (h >= 'A' && h <= 'Z'){
        num = h - 'A' ;
    } else if (h >= 'a' && h <= 'z'){
        num = h - 'a';
    } else {
        num = 100;
    }
    return num;
}


int main(int argc, char **argv) { //arg[1] = encrypt/decrypt, arg[2] = key, arg[3] = text; arg[] is pointer, arg[][] is character

    int key = 2;
    int text = 3;
    //char *key = argv[3];
    if (argc < 4) {
        fprintf(stderr, "Error: Too little arguments.\n");
        return 1;
    }

    if (argv[key][0] == '\0') {
        printf("%s\n", argv[text]);
    }

    unsigned char let_key;
    unsigned char let_word;
    int len_word = strlen(argv[text]);
    int len_key = strlen(argv[key]);

    if(strcmp(argv[1], "encrypt") == 0){
        //encrypt
        int i = 0;
        int j = 0;
        let_word = argv[text][0];
        while (let_word != '\0'){
            if (j == len_key){
                j = 0;
            }
            let_word = argv[text][i];
            let_key = argv[key][j];
            int num_key = chartonum(let_key); //gives how many positions to rotate by
            while (num_key == 100){
                j = j+1; 
                let_key = argv[key][j];
                num_key = chartonum(let_key);
            }
            if (let_word >= 'A' && let_word <= 'Z'){
                let_word = let_word + num_key;
            } else if (let_word >= 'a' && let_word <= 'z'){
                let_word = let_word + num_key;
            } else {
                j= j-1;
            }
            printf("%c", let_word);
            i++;
            j++;
    }
    printf("\n");

    } else if(strcmp(argv[1], "decrypt") == 0){
        int i = 0;
        int j = 0;
        let_word = argv[text][0];
        while (let_word != '\0'){
            if (j == len_key){
                j = 0;
            }
            let_word = argv[text][i];
            let_key = argv[key][j];
            int num_key = chartonum(let_key); //gives how many positions to rotate by
            while (num_key == 100){
                j = j+1; 
                let_key = argv[key][j];
                num_key = chartonum(let_key);
            }
            if (let_word >= 'A' && let_word <= 'Z'){
                let_word = let_word + num_key;
            } else if (let_word >= 'a' && let_word <= 'z'){
                let_word = let_word - num_key;
            } else {
                j= j-1;
            }
            printf("%c", let_word);
            i++;
            j++;
        }
        printf("\n");

    } else {
        fprintf(stderr, "expected command encrypt or decrypt.\n");
        return 1;
    }

    return 0;
}
