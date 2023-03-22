#include "../commands.h"

void drop_handler(int param_id, int param_drop_id, int param_repl_id, int id,
                  int *left_id, int *right_id, void **left_pusher,
                  void **right_pusher, void *root_pusher, void *context) {
  char msg[100];
  if (param_id == id) {
    if (*left_id == param_drop_id) {
      zmq_close(*left_pusher);
      *left_pusher = NULL;
      *left_id = -1;
      if (param_repl_id != -1) {
        *left_pusher = init_socket(context, param_repl_id, ZMQ_PUSH);
        *left_id = param_repl_id;
      }
    } else if (*right_id == param_drop_id) {
      zmq_close(*right_pusher);
      *right_pusher = NULL;
      *right_id = -1;
      if (param_repl_id != -1) {
        *right_pusher = init_socket(context, param_repl_id, ZMQ_PUSH);
        *right_id = param_repl_id;
      }
    }
  } else {
    sprintf(msg, "drop %d %d %d", param_id, param_drop_id, param_repl_id);
    pass_cmd_down(*left_pusher, *right_pusher, msg, 100);
  }
}