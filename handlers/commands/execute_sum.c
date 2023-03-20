#include "../commands.h"

int sum_from_str(char *buffer);

void execute_sum_handler(char *buffer, int id, void *root_pusher,
                         void *sibling_pusher, void *son_pusher) {
  char cmd[100], *line = malloc(sizeof(char) * 1000), *line_ptr;
  char msg[100];
  int param_id, param_n, pos, num = 100500, result = 0, offset = 0;
  sscanf(buffer, "%s %d %d %n", cmd, &param_id, &param_n, &pos);
  if (id == param_id) {
    for (int i = 0; i < param_n; ++i) {
      if (!sscanf(buffer + pos, "%d%n", &num, &offset)) {
        sprintf(msg, "Bad arguments: wrong number of ints or bad ints\n");
        send_message(root_pusher, msg, 100);
        return;
      }
      result += num;
      pos += offset;
    }

    sprintf(msg, "%d", result);
    send_message(root_pusher, msg, 100);
  } else {
    pass_cmd_down(sibling_pusher, son_pusher, buffer, 1000);
  }
  free(line);
}
