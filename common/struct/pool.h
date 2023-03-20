#ifndef COMMON_STRUCT_POOL_H
#define COMMON_STRUCT_POOL_H

#include <assert.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../util/zmq_helpers.h"
#include "node.h"

typedef struct _pool {
  Node *root_node;
  void *root_respond;
  int nodes[1000];
  int nodes_count;
} Pool;

Pool *init_pool();
int child_to_pool(Pool *p, int child_id, int parent_id);
int child_from_pool(Pool *p, int id);

#endif  // COMMON_STRUCT_POOL_H