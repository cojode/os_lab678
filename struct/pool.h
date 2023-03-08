#ifndef STRUCT_POOL_H
#define STRUCT_POOL_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <zmq.h>

#include "node.h"

const int ROOT_ID = -1;
const int SRC_PORT = 2222;
const char SERVER_SOCKET_PATTERN[] = "tcp://localhost:";

typedef struct _pool {
  Node *root_node;
} Pool;

typedef struct _mq_handler {
  int id;
  void *context;
  void *responder;
  void *requester;
} Handler_mq;

Pool *init_pool();
int born_node(int parent_id, int child_id);
int child_to_pool(Pool *p, int child_id, int parent_id);

Handler_mq *init_handler_mq(int id);

#endif  // STRUCT_POOL_H