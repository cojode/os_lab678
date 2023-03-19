#include "child.h"

#include "../config/msg.h"
#include "../handlers/commands.h"

void imalive(int id);

int main(int argc, char *argv[]) {
  int id = atoi(argv[1]), parent_id = atoi(argv[2]), son_id = ROOT_ID,
      sibling_id = ROOT_ID;
  void *context = zmq_ctx_new();
  void *puller = init_socket(context, id, ZMQ_PULL);
  void *parent_pusher = init_socket(context, parent_id, ZMQ_PUSH);
  void *root_pusher = init_socket(context, ROOT_ID, ZMQ_PUSH);
  void *sibling_pusher = NULL;
  void *son_pusher = NULL;

  imalive(id);
  char buffer[1000];
  char subcommand[1000], cmd[1000];
  int size = 0, param_id, param_parent_id, param_sibling_id, param_son_id;

  clock_t timer_start, timer_stop;

  while (1) {
    size = zmq_recv(puller, buffer, sizeof(buffer) + 2, ZMQ_DONTWAIT);
    if (size != -1) {
      printf(NODE_PREFIX, id);
      printf(" Recieved command: %s\n", buffer);
      switch (solve_cmd(buffer)) {
        case EXIT: {
          printf("%s %s\n", ROOT_PREFIX, MSG_EXIT_SIGNAL_START);
          printf("%s", MSG_EXIT_SIGNAL_COMPLETE);
          return 1;
        }
        case UNKNOWN: {
          printf(NODE_PREFIX, id);
          printf("%s %s\n", ERR_PREFIX, ERR_UNKNOWN_CMD);
          break;
        }
        case CREATE: {
          sscanf(buffer, "%s %d %d", cmd, &param_id, &param_parent_id);
          create_handler(id, param_id, param_parent_id, context, &son_pusher,
                         &son_id, sibling_pusher, buffer, size);
          break;
        }
        case REMOVE: {
          sscanf(buffer, "%s %d", cmd, &param_id);
          remove_handler(id, sibling_id, son_id, param_id, sibling_pusher,
                         root_pusher, parent_pusher, son_pusher);
          break;
        }
        case EXECUTE: {
          sscanf(buffer, "%s %d %s", cmd, &param_id, subcommand);
          execute_handler(id, param_id, buffer, root_pusher, sibling_pusher,
                          son_pusher, &timer_start, &timer_stop);
          break;
        }
        case PING: {
          sscanf(buffer, "%s %d", cmd, &param_id);
          ping_client_handler(id, param_id, root_pusher, sibling_pusher,
                              son_pusher);
          break;
        }
        case SIBLING: {
          sscanf(buffer, "%s %d", cmd, &param_id);
          sibling_handler(param_id, &sibling_id, context, &sibling_pusher);
          break;
        }
        case DISCARD: {
          sscanf(buffer, "%s %d %d", cmd, &param_id, &param_son_id);
          discard_handler(param_id, param_son_id, sibling_id, &son_id, context,
                          &son_pusher);
          break;
        }
        case KICK: {
          sscanf(buffer, "%s %d %d", cmd, &param_id, &param_sibling_id);
          kick_handler(param_id, param_sibling_id, &sibling_id, &sibling_pusher,
                       context);
          break;
        }
      }
    }
  }
  printf("Bye");
  return 0;
}

void imalive(int id) { printf("%s %d\n", OK_PREFIX, getpid()); }
