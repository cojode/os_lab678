#include "pool.h"

Pool *init_pool() {
  Pool *p = (Pool *)malloc(sizeof(Pool));
  p->root_node = NULL;
  p->nodes_count = 0;
  return p;
}

int child_to_pool(Pool *p, int id, char *cmd) {
  if (!(p->root_node)) {
    p->root_node = init_node(id, NULL);
    p->nodes[p->nodes_count] = id;
    p->nodes_count++;
    return 2;
  }
  int result = add_node(p->root_node, id);
  if (result == 0) {
    p->nodes[p->nodes_count] = id;
    p->nodes_count++;
    Node *added_node = find_node(p->root_node, id);
    int parent_id = added_node->parent->id;
    int is_left = added_node->parent->left == added_node;
    sprintf(cmd, "create %d %d %d", id, parent_id, is_left);
  }
  return result;
}

int child_from_pool(Pool *p, int id, char *drop_cmd_1, char *drop_cmd_2,
                    char *child_cmd) {
  Node *node = find_node(p->root_node, id),
       *deepest = deepest_node(p->root_node), *left = node->left,
       *right = node->right;
  if (!node) return 1;
  int left_id, right_id;
  if (!left)
    left_id = -1;
  else
    left_id = left->id;
  if (!right)
    right_id = -1;
  else
    right_id = right->id;
  sprintf(drop_cmd_1, "drop %d %d %d", deepest->parent->id, deepest->id, -1);
  sprintf(drop_cmd_2, "drop %d %d %d", node->parent->id, id, deepest->id);
  sprintf(child_cmd, "child %d %d %d", deepest->id, left_id, right_id);
  remove_node(p->root_node, id);
  for (int i = 0; i < p->nodes_count; ++i) {
    if (p->nodes[i] == id) {
      int tmp = p->nodes[i];
      p->nodes[i] = p->nodes[p->nodes_count - 1];
      p->nodes[p->nodes_count - 1] = tmp;
      break;
    }
  }
  p->nodes_count--;
  printf("remaining nodes: ");
  for (int i = 0; i < p->nodes_count; ++i) printf("[%d] ", p->nodes[i]);

  return 0;
}
