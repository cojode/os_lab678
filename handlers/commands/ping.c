#include "../commands.h"

int ping_handler(int param_id, void *puller, void *son_pusher, char *buffer,
                 size_t size) {
  if (param_id == ROOT_ID) {
    printf("%s %s 1\n", ROOT_PREFIX, OK_PREFIX);
    return 1;
  } else {
    send_message(son_pusher, buffer, size);
    zmq_sleep(PING_TIMEOUT);
    return zmq_recv(puller, buffer, sizeof(buffer), ZMQ_DONTWAIT) != -1;
  }
}

void ping_client_handler(int id, int param_id, void *root_pusher,
                         void *sibling_pusher, void *son_pusher) {
  if (param_id == id) {
    send_message(root_pusher, "alive", 6);
  } else {
    char msg[100];
    sprintf(msg, "ping %d", param_id);
    pass_cmd_down(sibling_pusher, son_pusher, msg, sizeof(msg));
  }
}