#include <stdio.h>
#include <string.h>

char file_buffer[64*1024];

int get_context (int key_ptr){
  //get the starting location
  if (argv[3][0] == '0'){
    printf("%s", argv[2]);
  } else if (argv[3][0] == '1'){
    //do something
  }
}

int main(int argc, char **argv) {
  //Setup and Failure/Error code////////////////////////////////////////////////
    if (argc != 4) {
        fprintf(stderr, "usage: %s <file> <key> <lines before>\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Could not open %s: ", argv[1]);
        perror("");
        return 1;
    }
    int bytes_read = fread(file_buffer, 1, sizeof(file_buffer) - 1, f);
    if (!feof(f)) {
        fprintf(stderr, "Could not read entire file. Is it too big?\n");
        return 1;
    }
    fclose(f);
    // we want this to be a null-treminated string,
    // but fread just reads the file as binary, so we add it ourselves

    file_buffer[bytes_read] = '\0';
    for(int i = 0; i < sizeof(file_buffer); i++){
      printf("%c", file_buffer[i]);
    }

    int key = 2;
    int lines = 3;

    unsigned char let_key;
    int len = strlen(argv[key]);
    char keyword[len];
    for(int i = 0; i < len; i++){
      keyword[i] = argv[key][i];
      printf("%c", keyword[i]);
    }

    //Main code/////////////////////////////////////////////////////////////////

    ptr = strstr(line, argv[key]);


    return 0;
}
