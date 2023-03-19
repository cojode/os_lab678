#include "../commands.h"
/**
 * @brief This command makes kick a node from a family of sibling nodes
 */
void kick_handler(int param_id, int param_sibling_id, int *sibling_id,
                  void **sibling_pusher, void *context) {
  if (param_id == *sibling_id) {
    zmq_close(*sibling_pusher);
    sibling_pusher = NULL;
    if (param_sibling_id != ROOT_ID) {
      *sibling_pusher = init_socket(context, param_sibling_id, ZMQ_PUSH);
      *sibling_id = param_sibling_id;
    }
  } else {
    char msg[100];
    sprintf(msg, "kick %d %d", param_id, param_sibling_id);
    send_message(*sibling_pusher, msg, sizeof(msg));
  }
}