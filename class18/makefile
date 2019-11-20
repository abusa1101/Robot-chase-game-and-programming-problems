CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

message: message.c l2g_t.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm -O3 -I$(HOME)/local/include -L$(HOME)/local/lib -llcm
