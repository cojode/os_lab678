#include "../common/struct/node.h"

int main() {
  Node *tree = init_node(-1, NULL);
  int cmd, id, parent_id;
  while (scanf("%d", &cmd)) {
    switch (cmd) {
      case 1: {
        print_tree(tree, 0);
        break;
      }
      case 2: {
        scanf("%d", &id);
        switch (add_node(tree, id)) {
          case 0:
            printf("OK\n");
            break;
          case 1:
            printf("this id already in tree: %d\n", id);
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
            printf("this id is not in tree: %d\n", id);
            break;
        }
        break;
      }
      case 4: {
        printf("[%d]\n", deepest_node(tree)->id);
      }
    }
  }
  return 0;
}
