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
  SIBLING,
  DISCARD,
  KICK
};

enum SUBCMD_CODES { START = 1, STOP, TIME };

void create_handler(int id, int child_id, int param_parent_id, void *context,
                    void **son_pusher, int *son_id, void *sibling_pusher,
                    char *buffer, size_t size);
void ping_handler(int param_id, void *puller, void *son_pusher, char *buffer,
                  size_t size);
void discard_handler(int param_id, int param_son_id, int sibling_id,
                     int *son_id, void *context, void **son_pusher);
void kick_handler(int param_id, int param_sibling_id, int *sibling_id,
                  void **sibling_pusher, void *context);
void sibling_handler(int param_id, int *sibling_id, void *context,
                     void **sibling_pusher);
void ping_client_handler(int id, int param_id, void *root_pusher,
                         void *sibling_pusher, void *son_pusher);
void remove_handler(int id, int sibling_id, int son_id, int param_id,
                    void *sibling_pusher, void *root_pusher,
                    void *parent_pusher, void *son_pusher);
void execute_handler(int id, int param_id, char buffer[1000], void *root_pusher,
                     void *sibling_pusher, void *son_pusher,
                     clock_t *timer_start, clock_t *timer_stop);
int register_node(int parent_id, int child_id);
int solve_cmd(char *buffer);
int solve_subcmd(char *buffer);

void start_timer(time_t *start, time_t *stop);
void stop_timer(time_t *stop);
double time_timer(time_t start, time_t stop);

#endif  // HANDLERS_COMMANDS_H