SERVER=server/server.c
STRUCT=$(wildcard common/struct/*.c)
CMDS=$(wildcard handlers/commands/*.c)
UTIL=common/util/zmq_helpers.c
CLIENT=client/child.c
MSG=config/msg.c
SRCS= $(STRUCT) $(CMDS) $(UTIL) $(MSG)
ZMQ_FLAGS=-lzmq 
BIN_SERVER=bin/server
BIN_CLIENT=bin/child

all: clean bin/client bin/server
	./bin/server

bin/server:
	gcc $(SERVER) $(SRCS) -o $(BIN_SERVER) $(ZMQ_FLAGS)

bin/client:
	gcc $(CLIENT) $(SRCS) -o $(BIN_CLIENT) $(ZMQ_FLAGS)

clean:
	rm -f bin/*

test_tree:
	gcc struct/node.c test/test_node.c -o bin/test
	./bin/test