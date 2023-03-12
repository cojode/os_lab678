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
  DISCARD
};

void create_handler(int id, int child_id, int param_parent_id, void *context,
                    void **son_pusher, void *sibling_pusher, char *buffer,
                    size_t size);
void ping_handler(int param_id, void *puller, void *son_pusher, char *buffer,
                  size_t size);
int register_node(int parent_id, int child_id);
int solve_cmd(char *buffer);

#endif  // HANDLERS_COMMANDS_H