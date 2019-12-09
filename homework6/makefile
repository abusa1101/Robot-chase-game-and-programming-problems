CFLAGS = -ggdb3 -std=c11 -Wall -Wunused-parameter -Wstrict-prototypes -Werror -Wextra -Wshadow
CFLAGS += -Wno-sign-compare -Wno-unused-parameter -Wno-unused-variable
CFLAGS += -fsanitize=address -fsanitize=undefined

gui: gui.c image_server.c gx.c bmp.c collisions.c other_functions.c lcm_handle_async.c
	gcc -o $@ $^ $(CFLAGS) $(LCMTYPES_SOURCE) -lpthread -lm -llcm -O3 -I$(HOME)/local/include -L$(HOME)/local/lib

lcm_jar := $(shell dirname $(shell which lcm-gen))/../share/java/lcm.jar
lcmtypes.jar: lcmtypes/*.lcm
	lcm-gen -j lcmtypes/*.lcm
	javac lcmtypes/*.java -cp $(value lcm_jar)
	jar cf lcmtypes.jar lcmtypes/*.class

lcm-spy: lcmtypes.jar
	CLASSPATH=lcmtypes.jar lcm-spy >/dev/null

lcmtypes/%_t.c: lcmtypes/%_t.lcm
	lcm-gen -c --c-cpath lcmtypes/ --c-hpath lcmtypes/ $^

LCMTYPES_SOURCE = lcmtypes/settings_t.c lcmtypes/reset_t.c lcmtypes/world_t.c lcmtypes/agent_t.c
#lcmtypes/action_t.c
