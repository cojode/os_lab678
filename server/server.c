#include "../config/msg.h"
#include "../handlers/commands.h"

const int MAX_CMD_LENGTH = 100;

int solve_cmd(char *cmd);

int main() {
  void *context = zmq_ctx_new();
  // * Puller - passively listening socket
  // * It connects to the static addres, which is defined by node's id
  // * This setups unique address for each node, and every node could connect to
  // * another via id only
  void *puller = init_socket(context, ROOT_ID, ZMQ_PULL);
  // * Pusher - actively requesting socket
  // * In order to send message to other node, its connects to this node's
  // * address, holded by other node's responder, and transfer message to it
  void *son_pusher = NULL;
  int son_id = -1;

  int exit_status = 0, id, parent_id;
  char cmd[MAX_CMD_LENGTH];
  char buffer[1000];

  Pool *pool = init_pool();

  while (!exit_status) {
    printf(ROOT_AWAIT_CMD_PREFIX);
    scanf("%s", &cmd);
    switch (solve_cmd(cmd)) {
      case EXIT: {
        exit_status = 1;
        printf("%s %s\n", ROOT_PREFIX, MSG_EXIT_SIGNAL_START);
        printf(MSG_EXIT_SIGNAL_COMPLETE);
        break;
      }
      case UNKNOWN: {
        printf("%s %s %s", ROOT_PREFIX, ERR_PREFIX, ERR_UNKNOWN_CMD);
        break;
      }
      case CREATE: {
        scanf("%d", &id);
        scanf("%d", &parent_id);
        create_handler(pool, id, parent_id, context, &son_pusher);
        break;
      }
      case REMOVE: {
        break;
      }
      case EXECUTE: {
        break;
      }
      case PING: {
        strcpy(buffer, "hello");
        scanf("%d", &id);
        if (id == ROOT_ID) {
          printf("%s %s 1\n", ROOT_PREFIX, OK_PREFIX);
          break;
        }
        if (!find_node_by_id(pool->root_node, id)) {
          printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_NOT_FOUND);
          break;
        }
        printf("%s Pinging [#%d] node...\n", ROOT_PREFIX, id);
        send_message(son_pusher, buffer, 6);
        break;
      }
      case LS: {
        print_tree(pool->root_node);
        break;
      }
    }
    zmq_sleep(1);
  }
  zmq_close(puller);
  zmq_ctx_destroy(context);
  return 0;
}

int solve_cmd(char *cmd) {
  if (!strcmp("exit", cmd)) return EXIT;
  if (!strcmp("create", cmd)) return CREATE;
  if (!strcmp("remove", cmd)) return REMOVE;
  if (!strcmp("exec", cmd)) return EXECUTE;
  if (!strcmp("ping", cmd)) return PING;
  if (!strcmp("ls", cmd)) return LS;
  return UNKNOWN;
}
