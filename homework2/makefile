CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

rasterize: rasterize.c bmp.c vectors.c test.c image_server.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm

braitenberg: braitenberg.c bmp.c image_server.c gx.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm
