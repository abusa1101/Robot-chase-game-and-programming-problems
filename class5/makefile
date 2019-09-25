CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
#CFLAGS += -fsanitize=address -fsanitize=undefined

golomb: golomb.c
	gcc -o $@ $^ $(CFLAGS) -lm

gol: gol.c
	gcc -o $@ $^ $(CFLAGS) -lm
