CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

manual: manual.c bmp.c image_server.c chase_functions.c gx.c collisions.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm -O3
