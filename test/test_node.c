#include "../common/struct/node.h"

int main() {
  Node *tree = init_node(-1, NULL, NULL, NULL);
  int cmd, id, parent_id;
  while (scanf("%d", &cmd)) {
    switch (cmd) {
      case 1: {
        print_tree(tree);
        break;
      }
      case 2: {
        scanf("%d %d", &id, &parent_id);
        switch (add_node(tree, id, parent_id)) {
          case 0:
            printf("OK\n");
            break;
          case 1:
            printf("this id already in tree: %d\n", id);
            break;
          case 2:
            printf("parent not found: %d\n", parent_id);
            break;
        }
        break;
      }
      case 3: {
        scanf("%d", &id);
        switch (remove_node(tree, id)) {
          case 0:
            printf("OK\n");
            break;
          case 1:
            printf("no such node: %d\n", id);
            break;
        }
        break;
      }
      case 4: {
        scanf("%d", &id);
        Node *neighbour = find_neighbour(tree, id);
        if (!neighbour)
          printf("no\n");
        else
          printf("%d", neighbour->id);
      }
      case 5: {
        scanf("%d %d", &parent_id, &id);
        if (is_parent(tree, parent_id, id)) {
          printf("Yes\n");
        } else {
          printf("No\n");
        }
        break;
      }
    }
  }
  return 0;
}
