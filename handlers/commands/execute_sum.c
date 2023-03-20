#include "../commands.h"

int sum_from_str(char *buffer);

void execute_sum_handler(char *buffer, int id, void *root_pusher,
                         void *sibling_pusher, void *son_pusher) {
  char cmd[100], *line = malloc(sizeof(char) * 1000), *line_ptr;
  int param_id, param_n, pos, num = 100500, result = 0;
  sscanf(buffer, "%s %d %d %n", cmd, &param_id, &param_n, &pos);
  if (id == param_id) {
    printf("%s", buffer + pos);
    for (int i = 0; i < param_n; ++i) {
      sscanf(buffer + pos, "%d %n", &num, &pos);
      result += num;
    }
    char msg[100];
    sprintf(msg, "%d", result);
    send_message(root_pusher, msg, 100);
  } else {
    pass_cmd_down(sibling_pusher, son_pusher, buffer, 1000);
  }
  free(line);
}
