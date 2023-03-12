#include "commands.h"

int solve_cmd(char *buffer) {
  char cmd[100];
  sscanf(buffer, "%s", cmd);
  if (!strcmp("exit", cmd)) return EXIT;
  if (!strcmp("create", cmd)) return CREATE;
  if (!strcmp("remove", cmd)) return REMOVE;
  if (!strcmp("exec", cmd)) return EXECUTE;
  if (!strcmp("ping", cmd)) return PING;
  if (!strcmp("ls", cmd)) return LS;
  if (!strcmp("sibling", cmd)) return SIBLING;
  if (!strcmp("discard", cmd)) return DISCARD;
  return UNKNOWN;
}
