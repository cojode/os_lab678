#include "../commands.h"

const char *CLIENT_PATH = "../../bin/child";

void create_handler(int id, int child_id, int param_parent_id, void *context,
                    void **son_pusher, int *son_id, void *sibling_pusher,
                    char *buffer, size_t size) {
  if (param_parent_id == id) {
    register_node(param_parent_id, child_id);
    if (*son_pusher) {
      char msg[100];
      sprintf(msg, "sibling %d", child_id);
      send_message(*son_pusher, msg, sizeof(msg));
    } else {
      *son_pusher = init_socket(context, child_id, ZMQ_PUSH);
      if (son_id) *son_id = child_id;
    }
  } else {
    pass_cmd_down(sibling_pusher, *son_pusher, buffer, size);
  }
}

int register_node(int parent_id, int child_id) {
  char str_child_id[100], str_parent_id[100];
  sprintf(str_child_id, "%d", child_id);
  sprintf(str_parent_id, "%d", parent_id);
  char *args[] = {"child", str_child_id, str_parent_id, NULL};
  int child_pid = fork();
  if (child_pid == 0) {
    char path[256];
    realpath("bin/child", path);
    if (execvp(path, args) == -1) {
      perror("Execv err");
    }
  }
  return child_pid;
}
