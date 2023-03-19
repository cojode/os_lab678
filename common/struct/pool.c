#include "pool.h"

Pool *init_pool() {
  Pool *p = (Pool *)malloc(sizeof(Pool));
  p->root_node = init_node(ROOT_ID, NULL, NULL, NULL);
  p->nodes_count = 0;
  return p;
}

int child_to_pool(Pool *p, int child_id, int parent_id) {
  int result = add_node(p->root_node, child_id, parent_id);
  if (result == 0) {
    p->nodes[p->nodes_count] = child_id;
    p->nodes_count++;
  }
  return result;
}

int child_from_pool(Pool *p, int id) {
  int result = remove_node(p->root_node, id);
  if (result == 0) {
    p->nodes[p->nodes_count] = -1;
    p->nodes_count--;
  }
  return result;
}