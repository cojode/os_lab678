#include "../commands.h"

void sibling_handler(int param_id, int *sibling_id, void *context,
                     void **sibling_pusher) {
  if (*sibling_pusher) {
    char msg[100];
    sprintf(msg, "sibling %d", param_id);
    send_message(*sibling_pusher, msg, sizeof(msg));
  } else {
    *sibling_pusher = init_socket(context, param_id, ZMQ_PUSH);
    *sibling_id = param_id;
  }
}