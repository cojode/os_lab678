#ifndef HANDLERS_COMMANDS_H
#define HANDLERS_COMMANDS_H

#include "../common/struct/pool.h"
#include "../common/util/zmq_helpers.h"
#include "../config/msg.h"

enum CMD_CODES {
  EXIT = -1,
  UNKNOWN,
  CREATE,
  REMOVE,
  EXECUTE,
  PING,
  LS,
  CHILD,
  DROP,
  KILL
};

enum SUBCMD_CODES { START = 1, STOP, TIME };

typedef struct _heartbit_thread_args {
  void *root_pusher;
  int delay;
  int id;
} heartbit_thread_args;

typedef struct _heartbit_listener_args {
  void *puller;
  int delay;
  int nodes[1000];
  int nodes_count;
} heartbit_listener_args;

void create_handler(int id, int child_id, int param_parent_id,
                    int param_is_left, void *context, void **left_pusher,
                    int *left_id, void **right_pusher, int *right_id,
                    char *buffer, size_t size);
int ping_handler(int param_id, void *puller, void *son_pusher, char *buffer,
                 size_t size);
void ping_client_handler(int id, int param_id, void *root_pusher,
                         void *left_pusher, void *right_pusher);
void child_handler(int id, int param_id, int param_left, int param_right,
                   int *left_id, int *right_id, void **left_pusher,
                   void **right_pusher, void *context);
void execute_sum_handler(char *buffer, int id, void *root_pusher,
                         void *left_pusher, void *right_pusher);
void drop_handler(int param_id, int param_drop_id, int param_repl_id, int id,
                  int *left_id, int *right_id, void **left_pusher,
                  void **right_pusher, void *root_pusher, void *context);
int register_node(int parent_id, int child_id);
int solve_cmd(char *buffer);
int solve_subcmd(char *buffer);

void *heartbit_listener(void *args);

void start_timer(time_t *start, time_t *stop);
void stop_timer(time_t *stop);
double time_timer(time_t start, time_t stop);

#endif  // HANDLERS_COMMANDS_H