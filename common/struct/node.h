#ifndef COMMON_STRUCT_NODE_H
#define COMMON_STRUCT_NODE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _node {
  int id;
  struct _node *parent;
  struct _node *left;
  struct _node *right;
} Node;

enum mode { MAX, MIN };

Node *init_node(int id, Node *parent);
int height(Node *root_node, int mode);
int add_node(Node *root_node, int id);
void print_tree(Node *root_node, int depth);
Node *find_node(Node *root_node, int id);
Node *deepest_node(Node *root_node);
int remove_node(Node *root_node, int id);
void discard(Node *node, Node *replacement);

#endif  // COMMON_STRUCT_NODE_H