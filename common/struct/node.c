#include "node.h"

Node *init_node(int id, Node *parent, Node *sibling, Node *son) {
  Node *n = (Node *)malloc(sizeof(Node));
  n->id = id;
  n->parent = parent;
  n->sibling = sibling;
  n->son = son;
  return n;
}

Node *find_node_by_id(Node *root_node, int id) {
  Node *result = NULL;
  if (!root_node) return NULL;
  if (root_node->id == id) return root_node;
  if (root_node->sibling) {
    result = find_node_by_id(root_node->sibling, id);
    if (result) return result;
  }
  if (root_node->son) {
    result = find_node_by_id(root_node->son, id);
    if (result) return result;
  }
  return result;
}

Node *find_neighbour(Node *root_node, int id) {
  Node *node = find_node_by_id(root_node, id);
  Node *parent = node->parent;
  if (parent->son == node) return parent;
  Node *sib = parent->son;
  while (sib->sibling != node) sib = sib->sibling;
  return sib;
}

int add_node(Node *root_node, int id, int parent_id) {
  if (!root_node) return 3;
  if (find_node_by_id(root_node, id)) return 1;
  Node *parent = find_node_by_id(root_node, parent_id);
  if (!parent) return 2;
  if (!parent->son)
    parent->son = init_node(id, parent, NULL, NULL);
  else {
    Node *last_son = parent->son;
    while (last_son->sibling) last_son = last_son->sibling;
    last_son->sibling = init_node(id, parent, NULL, NULL);
  }
  return 0;
}

void clear_node(Node *root_node) {
  if (root_node) {
    if (root_node->sibling) clear_node(root_node->sibling);
    if (root_node->son) clear_node(root_node->son);
    free(root_node);
  }
}

int remove_node(Node *root_node, int id) {
  Node *r_node = find_node_by_id(root_node, id);

  if (!r_node) return 1;

  while (!is_leaf(root_node, id)) {
    if (r_node->son) {
      swap_nodes(root_node, r_node->id, r_node->son->id);
      r_node = r_node->son;
    }
    if (r_node->sibling) {
      swap_nodes(root_node, r_node->id, r_node->sibling->id);
      r_node = r_node->sibling;
    }
  }
  print_tree(root_node);

  free(r_node);

  return 0;
}

int is_leaf(Node *root_node, int id) {
  Node *node = find_node_by_id(root_node, id);
  if (!node) return 0;
  return !(node->sibling || node->son);
}

int swap_nodes(Node *root_node, int id_a, int id_b) {
  Node *node_a = find_node_by_id(root_node, id_a);
  Node *node_b = find_node_by_id(root_node, id_b);

  if (!(node_a && node_b)) return 1;
  printf("swap %d with %d\n", node_a->id, node_b->id);
  int tmp = node_a->id;
  node_a->id = node_b->id;
  node_b->id = tmp;

  return 0;
}

void print_tree_recursive(Node *node, int level) {
  while (node) {
    for (int i = 0; i < level; ++i) putchar('\t');
    printf("Node: %d\n", node->id);
    if (node->son) {
      for (int i = 0; i < level; ++i) putchar('\t');
      printf("Children:\n");
      print_tree_recursive(node->son, level + 1);
    }

    node = node->sibling;
  }
}

void print_tree(Node *root_node) { print_tree_recursive(root_node, 0); }