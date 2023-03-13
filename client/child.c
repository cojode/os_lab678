#include "child.h"

#include "../config/msg.h"
#include "../handlers/commands.h"

void imalive(int id);

int main(int argc, char *argv[]) {
  int id = atoi(argv[1]), parent_id = atoi(argv[2]);
  void *context = zmq_ctx_new();
  void *puller = init_socket(context, id, ZMQ_PULL);
  void *parent_pusher = init_socket(context, parent_id, ZMQ_PUSH);
  void *root_pusher = init_socket(context, ROOT_ID, ZMQ_PUSH);
  void *sibling_pusher = NULL;
  void *son_pusher = NULL;

  imalive(id);
  char buffer[1000];
  char subcommand[1000], cmd[1000];
  int size = 0, param_id, param_parent_id;

  clock_t timer_start, timer_stop;

  while (1) {
    size = zmq_recv(puller, buffer, sizeof(buffer) + 2, ZMQ_DONTWAIT);
    if (size != -1) {
      printf(NODE_PREFIX, id);
      printf(" Recieved command: %s\n", buffer);
      switch (solve_cmd(buffer)) {
        case EXIT: {
          printf("%s %s\n", ROOT_PREFIX, MSG_EXIT_SIGNAL_START);
          printf(MSG_EXIT_SIGNAL_COMPLETE);
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
                         sibling_pusher, buffer, size);
          break;
        }
        case REMOVE: {
          break;
        }
        case EXECUTE: {
          sscanf(buffer, "%s %d %s", cmd, &param_id, subcommand);
          char msg[100];
          if (param_id == id) {
            switch (solve_subcmd(buffer)) {
              case TIME:
                sprintf(msg, "%s%d:%.0lf", OK_PREFIX, id,
                        time_timer(timer_start, timer_stop));
                send_message(root_pusher, msg, sizeof(msg));
                break;
              case START:
                start_timer(&timer_start, &timer_stop);
                sprintf(msg, "%s%d", OK_PREFIX, id);
                send_message(root_pusher, msg, sizeof(msg));
                break;
              case STOP:
                stop_timer(&timer_stop);
                sprintf(msg, "%s%d", OK_PREFIX, id);
                send_message(root_pusher, msg, sizeof(msg));
                break;
            }
          } else {
            if (sibling_pusher) send_message(sibling_pusher, buffer, size);
            if (son_pusher) send_message(son_pusher, buffer, size);
          }
          break;
        }
        case PING: {
          sscanf(buffer, "%s %d", cmd, &param_id);
          if (param_id == id) {
            send_message(root_pusher, "alive", 6);
          } else {
            if (sibling_pusher) send_message(sibling_pusher, buffer, size);
            if (son_pusher) send_message(son_pusher, buffer, size);
          }
          break;
        }
        case SIBLING: {
          sscanf(buffer, "%s %d", cmd, &param_id);
          if (sibling_pusher)
            send_message(sibling_pusher, buffer, size);
          else
            sibling_pusher = init_socket(context, param_id, ZMQ_PUSH);
        }
      }
    }
  }
  printf("Bye");
  return 0;
}

void imalive(int id) { printf("%s %d\n", OK_PREFIX, getpid()); }
