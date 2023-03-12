#ifndef UTIL_ZMQ_HELPERS
#define UTIL_ZMQ_HELPERS

#include <stdlib.h>
#include <string.h>
#include <zmq.h>

extern const int ROOT_ID;
extern const int SRC_PORT;
extern const char SOCKET_PATTERN[];

// typedef struct _msg {
//   int cmd;
//   int id;
//   int parent_id;
//   int subcommand;
// } Message;

char *solve_address(int id);
void *init_socket(void *context, int id, int type);
int send_message(void *pusher, char *buffer, size_t size);
void pass_cmd_down(void *sibling_pusher, void *son_pusher, char *buffer,
                   size_t size);

#endif  // UTIL_ZMQ_HELPERS