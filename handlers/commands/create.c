#include "../commands.h"

const char *CLIENT_PATH = "../../bin/child";

int register_node(int parent_id, int child_id);

void create_handler(Pool *pool, int id, int parent_id, void *context,
                    void **son_pusher) {
  switch (add_node(pool->root_node, id, parent_id)) {
    case 0:
      printf("%s %s %s\n", ROOT_PREFIX, OK_PREFIX, MSG_NODE_CHECKED);
      if (parent_id == ROOT_ID) {
        register_node(ROOT_ID, id);
        *son_pusher = init_socket(context, id, ZMQ_PUSH);
      } else {
        send_message(son_pusher, "create", 7);
      }
      break;
    case 1:
      printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_ALREADY_EXISTS);
      break;
    case 2:
      printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_NO_PARENT);
      break;
  }
}

int register_node(int parent_id, int child_id) {
  char str_child_id[100], str_parent_id[100];
  sprintf(str_child_id, "%d", child_id);
  sprintf(str_parent_id, "%d", parent_id);
  char *args[] = {"child", str_child_id, str_parent_id, NULL};
  int child_pid = fork();
  if (child_pid == 0) {
    if (execvp("/home/cojod/work_dir/os/lab678/bin/child", args) == -1) {
      perror("Execv err");
    }
  }
  return child_pid;
}