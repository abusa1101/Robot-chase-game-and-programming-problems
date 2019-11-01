CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
#CFLAGS += -fsanitize=address -fsanitize=undefined

hash: hash.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm

hashcomp: hashcomp.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm -O3
