#include "pool.h"

Pool *init_pool() {
  Pool *p = (Pool *)malloc(sizeof(Pool));
  p->root_node = init_node(ROOT_ID, NULL, NULL, NULL);
  return p;
}

int child_to_pool(Pool *p, int child_id, int parent_id) {
  return add_node(p->root_node, child_id, parent_id);
}
