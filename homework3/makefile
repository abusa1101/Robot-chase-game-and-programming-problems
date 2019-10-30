CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

calc: calc.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm

chase: chase.c bmp.c image_server.c chase_functions.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm
