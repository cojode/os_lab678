#include "../struct/node.h"
#include "../struct/pool.h"
#include "../zen.h"

int main(int argc, char *argv[]) {
  void *context = zmq_ctx_new();
  void *requester = zmq_socket(context, ZMQ_REQ);
  int id = atoi(argv[1]);
  zmq_connect(requester, "tcp://localhost:5555");

  printf("%s %d\n", OK_PREFIX, getpid());
  printf("My id: %s", argv[1]);
  return 0;
}
