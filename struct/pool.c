#include "pool.h"

Pool *init_pool() {
  Pool *p = (Pool *)malloc(sizeof(Pool));
  p->root_node = init_node(ROOT_ID, NULL, NULL, NULL);
  return p;
}

// function to call inside child when create command reaches
int born_node(int parent_id, int child_id) {
  printf("Creating child process...\n");
  char str_child_id[100], str_parent_id[100];
  sprintf(str_child_id, "%d", child_id);
  sprintf(str_parent_id, "%d", parent_id);
  char *args[] = {"child", str_child_id, str_parent_id, NULL};
  int child_pid = fork();
  if (child_pid == 0) {
    if (execvp("/home/cojod/work_dir/os/lab678/bin/child", args) == -1) {
      perror("Execv err");
    }
  }
  return child_pid;
}

// f to call after succesful creating child process
int child_to_pool(Pool *p, int child_id, int parent_id) {
  return add_node(p->root_node, child_id, parent_id);
}

Handler_mq *init_handler_mq(int id) {
  Handler_mq *hmq = (Handler_mq *)malloc(sizeof(Handler_mq));
  hmq->id = id;
  hmq->context = zmq_ctx_new();
  hmq->requester = zmq_connect(context, )
}