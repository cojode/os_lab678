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

int add_node(Node *root_node, int id, int parent_id) {
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
  r_node->parent->son = r_node->sibling;
  Node *poor_children = r_node->son, *last_son = r_node->sibling;
  while (last_son->sibling) last_son = last_son->sibling;
  last_son->sibling = poor_children;
  free(r_node);
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