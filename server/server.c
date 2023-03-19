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
  int id = ROOT_ID, pid, son_id = -1;

  Pool *pool = init_pool();

  while (1) {
    int param_id, param_parent_id;
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
        sscanf(buffer, "%s %d %d", cmd, &param_id, &param_parent_id);
        switch (child_to_pool(pool, param_id, param_parent_id)) {
          case 0:
            // ! ping
            create_handler(ROOT_ID, param_id, param_parent_id, context,
                           &son_pusher, &son_id, NULL, buffer, sizeof(buffer));
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
        switch (child_from_pool(pool, param_id)) {
          case 0:
            if (param_id == id) {
              printf("%s", OK_PREFIX);
              return 0;
            }
            if (is_parent(pool->root_node, ROOT_ID, param_id)) {
              if (param_id == son_id) {
                printf("Direct son\n");
              } else {
                printf("Indirect son\n");
              }
            } else {
              printf("Not a son\n");
              send_message(son_pusher, buffer, sizeof(buffer));
              zmq_recv(puller, buffer, sizeof(buffer), 0);
              sscanf(buffer, "%s %d", cmd, &pid);
              printf("Killing %d\n", pid);
              kill(pid, SIGKILL);
            }
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
        printf(
            "%s %s %d\n", ROOT_PREFIX, OK_PREFIX,
            ping_handler(param_id, puller, son_pusher, buffer, sizeof(buffer)));
        break;
      }
      case LS: {
        print_tree(pool->root_node);
        break;
      }
      case PINGALL: {
        int no_response[100], len_nr = 0;
        for (int i = 0; i < pool->nodes_count; ++i) {
          char msg[100];
          sprintf(msg, "ping %d", pool->nodes[i]);
          if (!ping_handler(pool->nodes[i], puller, son_pusher, msg,
                            sizeof(msg))) {
            no_response[len_nr] = pool->nodes[i];
            len_nr++;
          }
        }
        if (len_nr) {
          printf("%s %s", ROOT_PREFIX, OK_PREFIX);
          for (int i = 0; i < len_nr; ++i) {
            printf("%d", no_response[i]);
            if (i != len_nr - 1) putchar(';');
          }
          putchar('\n');
        } else {
          printf("%s %s -1\n", ROOT_PREFIX, OK_PREFIX);
        }
        break;
      }
    }
  }
  zmq_close(puller);
  if (son_pusher) zmq_close(son_pusher);
  zmq_ctx_destroy(context);
  return 0;
}
