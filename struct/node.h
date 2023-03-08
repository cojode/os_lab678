#ifndef NODE_H
#define NODE_H

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
int add_node(Node *root_node, int id, int parent_id);
void clear_node(Node *root_node);
int remove_node(Node *root_node, int id);
void print_tree_recursive(Node *node, int level);
void print_tree(Node *root_node);

#endif  // NODE_H