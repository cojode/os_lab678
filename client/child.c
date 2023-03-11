#include "child.h"

#include "../config/msg.h"
#include "../handlers/commands.h"

void imalive(int id);

int main(int argc, char *argv[]) {
  int id = atoi(argv[1]), parent_id = atoi(argv[2]);
  void *context = zmq_ctx_new();
  void *puller = init_socket(context, id, ZMQ_PULL);
  void *parent_pusher = init_socket(context, parent_id, ZMQ_PUSH);
  void *sibling_pusher = NULL;
  void *son_pusher = NULL;

  imalive(id);
  char buffer[1000] = "";
  int size = 0;

  while (1) {
    recieve_message(puller, buffer);
    printf(NODE_PREFIX, id);
    printf(buffer);
    zmq_sleep(2);
  }
  printf("Bye");
  return 0;
}

void imalive(int id) {
  printf(NODE_PREFIX, id);
  printf(" %s %d\n", OK_PREFIX, getpid());
  printf(NODE_PREFIX, id);
  printf(" Listening on address %s\n", solve_address(id));
}
