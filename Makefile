CFLAG = -Wall -g -I include -MD

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst src/%.c, obj/%.o, ${SRCS})
DEPS = $(OBJS:.o=.d)

all: flow_compute_server

-include $(DEPS) #這行如果放在 all 之前，修改.h檔後，不會偵測到改變

# GMP GUN多重精度算術庫
flow_compute_server: $(OBJS)
	g++ $(CFLAG) -o $@ $^ -lgmp

obj/%.o: src/%.c
	g++ $(CFLAG) -c $< -o $@


.PHONY: clean run re

clean: 
	rm -f flow_compute_server obj/*.o obj/*.d

run: 
#有必要的話 先調整 sysctl -w fs.mqueue.msg_max=4096
	./flow_compute_server 1 192.168.2.2 1230 1231 1232 700 1024

re:
	make clean
	make
	make run
