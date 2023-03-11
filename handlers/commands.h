#ifndef HANDLERS_COMMANDS_H
#define HANDLERS_COMMANDS_H

#include "../common/struct/pool.h"
#include "../common/util/zmq_helpers.h"
#include "../config/msg.h"

void create_handler(Pool *pool, int id, int parent_id, void *context,
                    void **son_pusher);

#endif  // HANDLERS_COMMANDS_H