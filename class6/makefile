CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
<<<<<<< HEAD
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
#CFLAGS += -fsanitize=address -fsanitize=undefined

linked: linked.c
	gcc -o $@ $^ $(CFLAGS) -lm
=======
CFLAGS += -fsanitize=signed-integer-overflow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
#CFLAGS += -fsanitize=address -fsanitize=undefined
#gcc -o $@ $^ $(CFLAGS)
linked: linked.c
		gcc -o $@ $^ $(CFLAGS)
		#gcc -o linked linked.c
>>>>>>> 34afee1851efd6c26699b45918b1eb0331429daa
