#include "../commands.h"

void execute_timer_handler(int id, int param_id, char buffer[1000],
                           void *root_pusher, void *left_pusher,
                           void *right_pusher, clock_t *timer_start,
                           clock_t *timer_stop) {
  char msg[100];
  if (param_id == id) {
    switch (solve_subcmd(buffer)) {
      case TIME:
        sprintf(msg, "%s%d:%.0lf", OK_PREFIX, id,
                time_timer(*timer_start, *timer_stop));
        send_message(root_pusher, msg, sizeof(msg));
        break;
      case START:
        start_timer(timer_start, timer_stop);
        sprintf(msg, "%s%d", OK_PREFIX, id);
        send_message(root_pusher, msg, sizeof(msg));
        break;
      case STOP:
        stop_timer(timer_stop);
        sprintf(msg, "%s%d", OK_PREFIX, id);
        send_message(root_pusher, msg, sizeof(msg));
        break;
    }
  } else {
    pass_cmd_down(left_pusher, right_pusher, buffer, 1000);
  }
}
