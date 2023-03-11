#ifndef COMMON_STRUCT_POOL_H
#define COMMON_STRUCT_POOL_H

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../util/zmq_helpers.h"
#include "node.h"

typedef struct _pool {
  Node *root_node;
  void *root_respond;
} Pool;

Pool *init_pool();
int child_to_pool(Pool *p, int child_id, int parent_id);

#endif  // COMMON_STRUCT_POOL_H