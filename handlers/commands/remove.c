#include "../commands.h"

void remove_handler(int id, int sibling_id, int son_id, int param_id,
                    void *sibling_pusher, void *root_pusher,
                    void *parent_pusher, void *son_pusher) {
  if (param_id == id) {
    if (son_pusher) {
    } else {
      char msg[100];
      sprintf(msg, "discard %d %d", id, sibling_id);
      send_message(parent_pusher, msg, sizeof(msg));
      sprintf(msg, "kill %d", getpid());
      send_message(root_pusher, msg, sizeof(msg));
    }
  } else {
    char msg[100];
    sprintf(msg, "remove %d", param_id);
    pass_cmd_down(sibling_pusher, son_pusher, msg, sizeof(msg));
  }
}