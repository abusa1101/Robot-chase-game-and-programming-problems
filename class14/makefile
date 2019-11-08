CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
#CFLAGS += -fsanitize=address -fsanitize=undefined

helloworlds: helloworlds.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm

parallelhashcomp: parallelhashcomp.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm -O3
