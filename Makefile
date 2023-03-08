SERVER=server.c
STRUCT=struct/node.c
CMDS=$(wildcard commands/*.c)
CLIENT=client/child.c
SRCS= $(SERVER) $(STRUCT) $(CMDS)
ZMQ_FLAGS=-lzmq 
BIN_SERVER=bin/server
BIN_CLIENT=bin/child

all: clean bin/client bin/server
	./bin/server

bin/server:
	gcc $(SRCS) -o $(BIN_SERVER) $(ZMQ_FLAGS)

bin/client:
	gcc $(CLIENT) -o $(BIN_CLIENT) $(ZMQ_FLAGS)

clean:
	rm -f bin/*

test_tree:
	gcc struct/node.c test/test_node.c -o bin/test
	./bin/test