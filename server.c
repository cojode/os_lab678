#include "zen.h"

int solve_cmd(char *cmd);

int main() {
  void *context = zmq_ctx_new();
  void *responder = zmq_socket(context, ZMQ_REP);
  int rc = zmq_bind(responder, "tcp://localhost:1");
  int exit_status = 0;
  char cmd[MAX_CMD_LENGTH];
  scanf("%s", &cmd);
  while (!exit_status) {
    switch (solve_cmd(cmd)) {
      case EXIT: {
        exit_status = 1;
        break;
      }
      case UNKNOWN: {
        printf("%s Unknown command: %s\n", ERR_PREFIX, cmd);
        break;
      }
      case CREATE: {
        break;
      }
      case REMOVE: {
        break;
      }
      case TIMER: {
        break;
      }
      case PING: {
        break;
      }
    }
    scanf("%s", &cmd);
  }
  zmq_close(responder);
  zmq_ctx_destroy(context);
  return 0;
}

int solve_cmd(char *cmd) {
  if (!strcmp("exit", cmd)) return EXIT;
  if (!strcmp("create", cmd)) return CREATE;
  if (!strcmp("remove", cmd)) return REMOVE;
  if (!strcmp("exec", cmd)) return TIMER;
  if (!strcmp("ping", cmd)) return PING;
  return UNKNOWN;
}
