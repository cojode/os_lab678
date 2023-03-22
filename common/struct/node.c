#include "node.h"

Node *init_node(int id, Node *parent) {
  Node *n = (Node *)malloc(sizeof(Node));
  n->id = id;
  n->left = NULL;
  n->right = NULL;
  n->parent = parent;
  return n;
}

int height(Node *root_node, int mode) {
  if (!root_node) return -1;
  int left_height = height(root_node->left, mode);
  int right_height = height(root_node->right, mode);
  if (mode == MAX) {
    if (left_height > right_height)
      return left_height + 1;
    else
      return right_height + 1;
  } else if (mode == MIN) {
    if (left_height <= right_height)
      return left_height + 1;
    else
      return right_height + 1;
  }
}

int add_node(Node *root_node, int id) {
  if (!root_node) return -1;
  if (find_node(root_node, id)) return 1;
  int left_height = height(root_node->left, MAX),
      right_height = height(root_node->right, MAX);
  if (left_height < right_height) {
    if (root_node->left) return add_node(root_node->left, id);
    root_node->left = init_node(id, root_node);
  } else if (left_height > right_height) {
    if (root_node->right) return add_node(root_node->right, id);
    root_node->right = init_node(id, root_node);
  } else {
    left_height = height(root_node->left, MIN);
    right_height = height(root_node->right, MIN);
    if (left_height <= right_height) {
      if (root_node->left) return add_node(root_node->left, id);
      root_node->left = init_node(id, root_node);
    } else {
      if (root_node->right) return add_node(root_node->right, id);
      root_node->right = init_node(id, root_node);
    }
  }
  return 0;
}

void print_tree(Node *root_node, int depth) {
  if (!root_node) return;
  print_tree(root_node->right, depth + 1);
  for (int i = 0; i < depth; ++i) printf("   ");
  printf("[%d]\n", root_node->id);
  print_tree(root_node->left, depth + 1);
}

Node *find_node(Node *root_node, int id) {
  Node *search = NULL;
  if (!root_node) return search;
  if (root_node->id == id) return root_node;
  if (root_node->left) search = find_node(root_node->left, id);
  if (root_node->right && !search) search = find_node(root_node->right, id);
  return search;
}

Node *deepest_node(Node *root_node) {
  if (!root_node) return NULL;
  int left_height = height(root_node->left, MAX),
      right_height = height(root_node->right, MAX);
  if (left_height <= right_height) {
    if (root_node->right) return deepest_node(root_node->right);
  } else {
    if (root_node->left) return deepest_node(root_node->left);
  }
  return root_node;
}

int remove_node(Node *root_node, int id) {
  Node *r_node = find_node(root_node, id);
  if (!r_node) return 1;
  Node *parent = r_node->parent;
  Node *deepest = deepest_node(root_node);
  discard(deepest, NULL);
  discard(r_node, deepest);
  deepest->left = r_node->left;
  deepest->right = r_node->right;
  free(r_node);
  return 0;
}

/**
 * @brief isolates node from tree
 *
 * @param node which node to isolate
 * @param replacement replacement node for isolated node
 */
void discard(Node *node, Node *replacement) {
  Node *parent = node->parent;
  if (parent->right == node) {
    parent->right = replacement;
  } else if (parent->left == node) {
    parent->left = replacement;
  }
  if (replacement) replacement->parent = parent;
}
