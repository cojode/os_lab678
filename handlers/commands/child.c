#include "../commands.h"

void child_handler(int id, int param_id, int param_left, int param_right,
                   int *left_id, int *right_id, void **left_pusher,
                   void **right_pusher, void *context) {
  int pid = -1;
  if (param_id == id) {
    *left_id = param_left;
    *right_id = param_right;
    zmq_close(*left_pusher);
    zmq_close(*right_pusher);
    if (*left_id != -1) *left_pusher = init_socket(context, *left_id, ZMQ_PUSH);
    if (*right_id != -1)
      *right_pusher = init_socket(context, *right_id, ZMQ_PUSH);
  } else {
    char cmd[100];
    sprintf(cmd, "child %d %d %d", param_id, param_left, param_right);
    pass_cmd_down(*left_pusher, *right_pusher, cmd, 100);
  }
}