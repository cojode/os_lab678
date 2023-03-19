#ifndef COMMON_STRUCT_NODE_H
#define COMMON_STRUCT_NODE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _node {
  int id;
  struct _node *parent;
  struct _node *sibling;
  struct _node *son;
} Node;

Node *init_node(int id, Node *parent, Node *sibling, Node *son);
Node *find_node_by_id(Node *root_node, int id);
Node *find_neighbour(Node *root_node, int id);
int swap_nodes(Node *root_node, int id_a, int id_b);
int is_leaf(Node *root_node, int id);
int add_node(Node *root_node, int id, int parent_id);
void clear_node(Node *root_node);
int remove_node(Node *root_node, int id);
void print_tree_recursive(Node *node, int level);
void print_tree(Node *root_node);
Node *sibling_search(Node *first_sibling, Node *search, Node *new_parent);
int is_parent(Node *root_node, int parent_id, int node_id);

#endif  // COMMON_STRUCT_NODE_H