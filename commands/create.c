#include "../struct/pool.h"
#include "server_handlers.h"

const char *CLIENT_PATH = "../bin/child";

int create_handler(Pool *p, int id, int parent_id) {
  add_node(p->root_node, id, parent_id);
}