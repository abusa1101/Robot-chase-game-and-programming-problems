#include <stdio.h>
#include <string.h>

int chartonum(char h) //int num_key should be 0,1,2 for a,b,c and so on 
{ 
    if (h == '\0') { //to detect null character = blank pwd
        return 300;
    }
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
    if (argc != 4) {
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
        int i = 0; //text pointer
        int j = 0; //key pointer
        //int k = 0;
        let_word = argv[text][0]; // initialize let_word
        //printf("%c\n", let_word);
        //printf("%d\n", len_word);
        while (i <= len_word-1) { //loop lasts over the length of the text word
            if (j == len_key) {
                j = 0; //in order to repeat the key over the length of the word
            }
            let_word = argv[text][i]; //point to letter of word with text pointer
            let_key = argv[key][j]; // point to letter of key with key pointer
            int num_key = chartonum(let_key); //gives how many positions to rotate by
            if (num_key != 300){ //if key letter is NOT blank
                while (num_key == 100){ //if the key letter is a character
                    j = j+1; //change the position of the key pointer until it skips to the next alphabet
                    let_key = argv[key][j]; 
                    num_key = chartonum(let_key); //recalculate position to move to
                }
                if (let_word >= 'A' && let_word <= 'Z'){ //if alphabet, 
                    let_word = let_word + num_key; //move position
                } else if (let_word >= 'a' && let_word <= 'z'){ //if alphabet, move position
                    let_word = let_word + num_key; //move position
                } else { //if the text letter is a character, 
                    j= j-1; //retain the position of the key pointer until it reaches an alphabet
                }
                //printf("%c\n", let_word); //print the transformed text word
                i++;
                j++;
                //k++;
            }
            //let_word = argv[text][i];
            //i++;
            printf("%c", let_word);
        }
        //printf("%c\n", let_word);
        //printf("\n");
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
            if (num_key != 300){
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
        }
        printf("\n");
    } else {
        fprintf(stderr, "expected command encrypt or decrypt.\n");
        return 1;
    }

    return 0;
}
