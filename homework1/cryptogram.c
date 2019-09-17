#include <stdio.h>
#include <string.h>

void encrypt()
{


    
}

void decrypt()
{
    printf("NOOOOOO\n");
    /*unsigned int i;
    for(i=0;i<strlen(password);++i)
    {
        password[i] = password[i] + key;
    }*/
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
    unsigned char let_key;
    unsigned char let_word;
    int len_word = strlen(argv[text]);
    int len_key = strlen(argv[key]);
    int enc_state = 1;
    int dec_state = 1;
    char enc[] = "encrypt";
    char dec[] = "decrypt";
    int len_encdec = strlen(enc);
    int len_method = strlen(argv[1]);

    if (len_method == len_encdec) {
        for (int k = 0; k < len_method; k++) {
            if( argv[1][k] != enc[k]){ 
                enc_state = 0;
            }
        }
        for (int k = 0; k < len_method; k++) {
            if (argv[1][k] != dec[k]){
                dec_state = 0;
            } 
        }
        if (enc_state == 0 &&  dec_state == 0) {
            printf("expected command encrypt or decrypt. got '%s'\n", argv[1]);
            return 1;
        }
    } else {
        printf("expected command encrypt or decrypt. got '%s'\n", argv[1]);
        return 1;
    }

    if(enc_state == 1) {
        //encrypt();
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



    } else if  (dec_state == 1){
        decrypt();
    }

    return 0;
}
