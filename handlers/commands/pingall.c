#include "../commands.h"

int cmpfunc(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

void pingall_handler(void *puller, void *tree_pusher, int nodes_count,
                     int nodes[]) {
  send_message(tree_pusher, "pingall", 8);
  zmq_sleep(PING_TIMEOUT);
  char buffer[100], cmd[100];
  int alive[nodes_count], current_alive_id, alive_count = 0, dead_count = 0;

  while (zmq_recv(puller, buffer, sizeof(buffer), ZMQ_DONTWAIT) != -1) {
    sscanf(buffer, "%s %d", cmd, &current_alive_id);
    printf("recv: %s\n", buffer);
    alive[alive_count] = current_alive_id;
    alive_count++;
  }
  int merge[nodes_count + alive_count], dead[nodes_count];
  for (int i = 0; i < nodes_count; ++i) merge[i] = nodes[i];
  for (int i = 0; i < alive_count; ++i) merge[i + nodes_count] = alive[i];
  for (int i = 0; i < alive_count + nodes_count; ++i) printf("[%d] ", merge[i]);
  qsort(merge, nodes_count + alive_count, sizeof(int), cmpfunc);
  for (int i = 0; i < nodes_count + alive_count; ++i) {
    if (merge[i] == merge[i + 1]) {
      ++i;
    } else {
      dead[dead_count] = merge[i];
      dead_count++;
    }
  }
  printf("%s %s ", ROOT_PREFIX, OK_PREFIX);
  if (dead_count) {
    for (int i = 0; i < dead_count; ++i) printf("%d;", dead[i]);
    putchar('\n');
  } else {
    printf("-1\n");
  }
}
