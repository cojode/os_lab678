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
  void *tree_pusher = NULL;
  int id = ROOT_ID, pid, tree_first_id = -1;

  Pool *pool = init_pool();
  pthread_t tid = 0;

  while (1) {
    int param_id, param_parent_id, param_delay;
    char cmd[MAX_CMD_LENGTH];
    char subcommand[MAX_CMD_LENGTH];
    char buffer[BUFFER_SIZE];
    printf("%s", ROOT_AWAIT_CMD_PREFIX);
    fgets(buffer, sizeof(buffer), stdin);
    switch (solve_cmd(buffer)) {
      case EXIT: {
        printf("%s %s\n", ROOT_PREFIX, MSG_EXIT_SIGNAL_START);
        printf("%s", MSG_EXIT_SIGNAL_COMPLETE);
        return 1;
      }
      case UNKNOWN: {
        printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_UNKNOWN_CMD);
        break;
      }
      case CREATE: {
        sscanf(buffer, "%s %d", cmd, &param_id);
        if (param_id == ROOT_ID)
          printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_ROOT_ID);
        switch (child_to_pool(pool, param_id, buffer)) {
          case 0:
            send_message(tree_pusher, buffer, sizeof(buffer));
            break;
          case 1:
            printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_ALREADY_EXISTS);
            break;
          case 2:
            register_node(-1, param_id);
            tree_pusher = init_socket(context, param_id, ZMQ_PUSH);
            tree_first_id = param_id;
            printf("%s %s %d\n", ROOT_PREFIX, OK_PREFIX, param_id);
            break;
        }
        break;
      }
      case REMOVE: {
        char drop_cmd_1[100];
        char drop_cmd_2[100];
        char child_cmd[100];
        sscanf(buffer, "%s %d", cmd, &param_id);
        switch (child_from_pool(pool, param_id, drop_cmd_1, drop_cmd_2,
                                child_cmd)) {
          case 0:
            send_message(tree_pusher, drop_cmd_1, 100);
            send_message(tree_pusher, drop_cmd_2, 100);
            send_message(tree_pusher, child_cmd, 100);
            void *direct_pusher = init_socket(context, param_id, ZMQ_PUSH);
            send_message(direct_pusher, buffer, 100);
            zmq_recv(puller, buffer, sizeof(buffer), 0);
            sscanf(buffer, "%s %d", cmd, &pid);
            kill(pid, SIGKILL);
            printf("%s %s\n", ROOT_PREFIX, OK_PREFIX);
            if (direct_pusher) zmq_close(direct_pusher);
            break;
          case 1:
            printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_NOT_FOUND);
            break;
          case 2:
            printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_REMOVE_ROOT);
            break;
        }
        break;
      }
      case EXECUTE: {
        sscanf(buffer, "%s %d %s", cmd, &param_id, subcommand);
        if (!find_node(pool->root_node, param_id)) {
          printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_NOT_FOUND);
          break;
        }
        send_message(tree_pusher, buffer, sizeof(buffer));
        zmq_sleep(PING_TIMEOUT);
        if (zmq_recv(puller, buffer, sizeof(buffer), ZMQ_DONTWAIT) != -1) {
          printf("%s %s\n", ROOT_PREFIX, buffer);
        } else {
          printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_UNAVAILABLE);
        }
        break;
      }
      case LS: {
        print_tree(pool->root_node, 0);
        break;
      }
      case PING: {
        sscanf(buffer, "%s %d", cmd, &param_id);
        if (!find_node(pool->root_node, param_id)) {
          printf("%s %s %s\n", ROOT_PREFIX, ERR_PREFIX, ERR_NOT_FOUND);
        } else {
          printf("%s %s %d\n", ROOT_PREFIX, OK_PREFIX,
                 ping_handler(param_id, puller, tree_pusher, buffer, 100));
        }
        break;
      }
    }
  }
  zmq_close(puller);
  if (tree_pusher) zmq_close(tree_pusher);
  zmq_ctx_destroy(context);
  return 0;
}