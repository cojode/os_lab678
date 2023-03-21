SERVER=server/server.c
STRUCT=$(wildcard common/struct/*.c)
CMDS=handlers/commands_handler.c $(wildcard handlers/commands/*.c) 
UTIL=common/util/zmq_helpers.c
CLIENT=client/child.c
MSG=config/msg.c
SRCS= $(STRUCT) $(CMDS) $(UTIL) $(MSG)
LPT_FLAGS=-lpthread
ZMQ_FLAGS=-lzmq 
BIN_SERVER=bin/server
BIN_CLIENT=bin/child

all: clean bin/client bin/server
	./bin/server

bin/server:
	mkdir -p bin
	gcc $(SERVER) $(SRCS) -o $(BIN_SERVER) $(ZMQ_FLAGS) -g $(LPT_FLAGS)

bin/client:
	mkdir -p bin
	gcc $(CLIENT) $(SRCS) -o $(BIN_CLIENT) $(ZMQ_FLAGS) -g $(LPT_FLAGS)

clean:
	rm -f bin/*

test_tree:
	gcc common/struct/node.c test/test_node.c -o bin/test -g
	./bin/test
