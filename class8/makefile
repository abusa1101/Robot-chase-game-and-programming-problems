CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
#CFLAGS += -fsanitize=address -fsanitize=undefined

bisection: bisection.c
	gcc -o $@ $^ $(CFLAGS) -lm

binarysearch: binarysearch.c
	gcc -o $@ $^ $(CFLAGS) -lm

fibonacci: fibonacci.c
	gcc -o $@ $^ $(CFLAGS) -lm
