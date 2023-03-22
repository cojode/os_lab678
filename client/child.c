#include "child.h"

#include "../config/msg.h"
#include "../handlers/commands.h"

void imalive(int id);

int main(int argc, char *argv[]) {
  int id = atoi(argv[1]), parent_id = atoi(argv[2]), left_id = ROOT_ID,
      right_id = ROOT_ID;
  void *context = zmq_ctx_new();
  void *puller = init_socket(context, id, ZMQ_PULL);
  void *parent_pusher = init_socket(context, parent_id, ZMQ_PUSH);
  void *root_pusher = init_socket(context, ROOT_ID, ZMQ_PUSH);
  void *left_pusher = NULL;
  void *right_pusher = NULL;

  imalive(id);
  char buffer[1000];
  char subcommand[1000], cmd[1000];
  int size = 0, param_id, param_parent_id, param_sibling_id, param_son_id,
      param_delay, param_is_left, param_replacement, param_drop_id, param_left,
      param_right;

  clock_t timer_start, timer_stop;

  while (1) {
    size = zmq_recv(puller, buffer, sizeof(buffer) + 2, ZMQ_DONTWAIT);
    if (size != -1) {
      printf("[#%d]", id);
      printf("Recieved command: %s\n", buffer);
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
          sscanf(buffer, "%s %d %d %d", cmd, &param_id, &param_parent_id,
                 &param_is_left);
          create_handler(id, param_id, param_parent_id, param_is_left, context,
                         &left_pusher, &left_id, &right_pusher, &right_id,
                         buffer, sizeof(buffer));
          break;
        }
        case DROP: {
          sscanf(buffer, "%s %d %d %d", cmd, &param_id, &param_drop_id,
                 &param_replacement);
          drop_handler(param_id, param_drop_id, param_replacement, id, &left_id,
                       &right_id, &left_pusher, &right_pusher, root_pusher,
                       context);
          break;
        }
        case EXECUTE: {
          sscanf(buffer, "%s %d %s", cmd, &param_id, subcommand);
          execute_sum_handler(buffer, id, root_pusher, left_pusher,
                              right_pusher);
          break;
        }
        case PING: {
          sscanf(buffer, "%s %d", cmd, &param_id);
          if (param_id == id) {
            send_message(root_pusher, "alive", 6);
          } else {
            char msg[100];
            sprintf(msg, "ping %d", param_id);
            pass_cmd_down(left_pusher, right_pusher, msg, sizeof(msg));
          }
          break;
        }
        case CHILD: {
          sscanf(buffer, "%s %d %d %d", cmd, &param_id, &param_left,
                 &param_right);
          child_handler(id, param_id, param_left, param_right, &left_id,
                        &right_id, &left_pusher, &right_pusher, context);
          break;
        }
      }
    }
  }
  return 0;
}

void imalive(int id) { printf("%s %d\n", OK_PREFIX, getpid()); }
