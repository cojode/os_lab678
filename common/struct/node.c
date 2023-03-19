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
  if (root_node->id == id) return 2;
  Node *r_node = find_node_by_id(root_node, id);
  if (!r_node) return 1;
  Node *parent = r_node->parent, *connected_sibling = NULL;
  if (r_node->son) {
    if (parent->son == r_node) {
      parent->son = r_node->son;
      connected_sibling = sibling_search(r_node->son, NULL, parent);
      connected_sibling->sibling = r_node->sibling;
    } else {
      connected_sibling = sibling_search(parent->son, r_node, NULL);
      connected_sibling->sibling = r_node->son;
      connected_sibling = sibling_search(parent->son, NULL, parent);
      connected_sibling->sibling = r_node->sibling;
    }
  } else {
    if (parent->son == r_node) {
      parent->son = r_node->sibling;
    } else {
      connected_sibling = sibling_search(parent->son, r_node, NULL);
      connected_sibling->sibling = r_node->sibling;
    }
  }
  free(r_node);
  return 0;
}

Node *sibling_search(Node *first_sibling, Node *search, Node *new_parent) {
  Node *current_sibling = first_sibling;
  if (new_parent) current_sibling->parent = new_parent;
  while (current_sibling && current_sibling->sibling != search) {
    current_sibling = current_sibling->sibling;
    if (new_parent) current_sibling->parent = new_parent;
  }
  return current_sibling;
}

int is_leaf(Node *root_node, int id) {
  Node *node = find_node_by_id(root_node, id);
  if (!node) return 0;
  return !(node->sibling || node->son);
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

int is_parent(Node *root_node, int parent_id, int node_id) {
  Node *parent = find_node_by_id(root_node, parent_id),
       *node = find_node_by_id(root_node, node_id);
  if (parent && node) {
    if (parent->son == node) return 1;
    Node *search = parent->son;
    while (search) {
      search = search->sibling;
      if (search == node) return 1;
    }
  }
  return 0;
}