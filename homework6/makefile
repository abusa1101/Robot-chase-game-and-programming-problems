CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

gui: gui.c gx.c bmp.c collisions.c other_functions.c lcmtypes/reset_t.c lcmtypes/settings_t.c
	gcc -o $@ $^ $(CFLAGS) -lpthread -lm -O3 -I$(HOME)/local/include -L$(HOME)/local/lib -llcm
