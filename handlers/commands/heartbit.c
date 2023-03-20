#include "../commands.h"

void *heartbit_thread(void *args);

int cmpfunc(const void *a, const void *b) { return (*(int *)a - *(int *)b); }

void heartbit_handler(void *root_pusher, int delay, int id,
                      void *sibling_pusher, void *son_pusher) {
  pthread_t tid;
  heartbit_thread_args *args;
  args->delay = delay;
  args->id = id;
  args->root_pusher = root_pusher;
  pthread_create(&tid, NULL, heartbit_thread, args);
  char msg[100];
  sprintf(msg, "heartbit %d", delay);
  pass_cmd_down(sibling_pusher, son_pusher, msg, sizeof(msg));
}

void *heartbit_thread(void *args) {
  heartbit_thread_args *unpacked_args = (heartbit_thread_args *)args;
  int id = unpacked_args->id, delay = unpacked_args->delay;
  void *root_pusher = unpacked_args->root_pusher;
  char msg[256];
  sprintf(msg, "alive %d", id);
  while (1) {
    send_message(root_pusher, msg, sizeof(msg));
    printf(">> imalive %d\n", id);
    zmq_sleep(delay);
  }
}

void *heartbit_listener(void *args) {
  heartbit_listener_args *unpacked_args = (heartbit_listener_args *)args;
  int delay = unpacked_args->delay, nodes_count = unpacked_args->nodes_count,
      current_alive_id, alive_count = 0, dead_count = 0;
  int alive[nodes_count];
  void *puller = unpacked_args->puller;
  char buffer[256], cmd[256];
  for (int i = 0; i < nodes_count; ++i) printf("%d ", unpacked_args->nodes[i]);
  int first_time = 1;
  while (1) {
    alive_count = 0;
    dead_count = 0;
    while (zmq_recv(puller, buffer, sizeof(buffer), ZMQ_DONTWAIT) != -1) {
      sscanf(buffer, "%s %d", cmd, &current_alive_id);
      alive[alive_count] = current_alive_id;
      alive_count++;
    }
    int merge[nodes_count + alive_count], dead[nodes_count];
    for (int i = 0; i < nodes_count; ++i) merge[i] = unpacked_args->nodes[i];
    for (int i = 0; i < alive_count; ++i) merge[i + nodes_count] = alive[i];
    qsort(merge, nodes_count + alive_count, sizeof(int), cmpfunc);
    for (int i = 0; i < nodes_count + alive_count; ++i) {
      if (merge[i] == merge[i + 1]) {
        ++i;
      } else {
        dead[dead_count] = merge[i];
        dead_count++;
      }
    }
    if (dead_count && !first_time) {
      printf("Dead nodes: ");
      for (int i = 0; i < dead_count; ++i) printf("%d ", dead[i]);
      putchar('\n');
    }
    first_time = 0;
    zmq_sleep(delay);
  }
}