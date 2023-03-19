#include "../commands.h"

/**
 * @brief This command make parent discard their child
 */
void discard_handler(int param_id, int param_son_id, int sibling_id,
                     int *son_id, void *context, void **son_pusher) {
  if (param_id == *son_id) {
    zmq_close(*son_pusher);
    *son_pusher = NULL;
    if (param_son_id != ROOT_ID) {
      *son_pusher = init_socket(context, param_son_id, ZMQ_PUSH);
      *son_id = param_son_id;
    }
  } else {
    char msg[100];
    sprintf(msg, "kick %d %d", param_id, sibling_id);
    send_message(*son_pusher, msg, sizeof(msg));
  }
}
