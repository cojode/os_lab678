#include "../config/msg.h"
#include "../handlers/commands.h"

const int MAX_CMD_LENGTH = 100;

int main() {
  void *context = zmq_ctx_new();
  // * Puller - passively listening socket
  // * It connects to the static addres, which is defined by node's id
  // * This setups unique address for each node, and every node could connect to
  // * another via id only
  void *puller = init_socket(context, ROOT_ID, ZMQ_PULL);
  // * Pusher - actively requesting socket
  // * In order to send message to other node, its connects to this node's
  // * address, holded by other node's puller, and transfer message to it
  void *son_pusher = NULL;
  int id = ROOT_ID;

  Pool *pool = init_pool();

  while (1) {
    int param_id, param_parent_id;
    char cmd[MAX_CMD_LENGTH];
    char subcommand[MAX_CMD_LENGTH];
    char buffer[BUFFER_SIZE];
    printf(ROOT_AWAIT_CMD_PREFIX);
    fgets(buffer, sizeof(buffer), stdin);
    switch (solve_cmd(buffer)) {
      case EXIT: {
        printf("%s %s\n", ROOT_PREFIX, MSG_EXIT_SIGNAL_START);
        printf(MSG_EXIT_SIGNAL_COMPLETE);
        return 1;
      }
      case UNKNOWN: {
        printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_UNKNOWN_CMD);
        break;
      }
      case CREATE: {
        sscanf(buffer, "%s %d %d", cmd, &param_id, &param_parent_id);
        switch (add_node(pool->root_node, param_id, param_parent_id)) {
          case 0:
            // ! ping
            create_handler(ROOT_ID, param_id, param_parent_id, context,
                           &son_pusher, NULL, buffer, sizeof(buffer));

            break;
          case 1:
            printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_ALREADY_EXISTS);
            break;
          case 2:
            printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_NO_PARENT);
            break;
        }
        break;
      }
      case REMOVE: {
        sscanf(buffer, "%s %d", cmd, &param_id);
        switch (remove_node(pool->root_node, param_id)) {
          case 0:
            if (param_id == id) {
              printf("%s", OK_PREFIX);
              return 0;
            }
            send_message(son_pusher, buffer, sizeof(buffer));
          case 1:
            printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_NOT_FOUND);
            break;
        }
        break;
      }
      case EXECUTE: {
        sscanf(buffer, "%s %d %s", cmd, &param_id, subcommand);
        if (!find_node_by_id(pool->root_node, param_id)) {
          printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_NOT_FOUND);
          break;
        }
        switch (solve_subcmd(buffer)) {
          case UNKNOWN:
            printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_UNKNOWN_CMD);
            break;
          default:
            send_message(son_pusher, buffer, sizeof(buffer));
            zmq_sleep(PING_TIMEOUT);
            if (zmq_recv(puller, buffer, sizeof(buffer), ZMQ_DONTWAIT) != -1) {
              printf("%s %s\n", ROOT_PREFIX, buffer);
            } else {
              printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_UNAVAILABLE);
            }

            break;
        }
        break;
      }
      case PING: {
        sscanf(buffer, "%s %d", cmd, &param_id);
        if (!find_node_by_id(pool->root_node, param_id)) {
          printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_NOT_FOUND);
          break;
        }
        ping_handler(param_id, puller, son_pusher, buffer, sizeof(buffer));
        break;
      }
      case LS: {
        print_tree(pool->root_node);
        break;
      }
    }
  }
  zmq_close(puller);
  if (son_pusher) zmq_close(son_pusher);
  zmq_ctx_destroy(context);
  return 0;
}
