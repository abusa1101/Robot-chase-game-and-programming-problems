CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
#CFLAGS += -fsanitize=address -fsanitize=undefined

bigrams: bigrams.c hashtable.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm -O3

test: test.c hashtable.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm -O3
