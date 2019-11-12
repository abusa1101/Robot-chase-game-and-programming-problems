CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

buffer: buffer.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm
