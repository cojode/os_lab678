#include "msg.h"

const char ROOT_AWAIT_CMD_PREFIX[] = ">";
const char OK_PREFIX[] = "Ok:";
const char ERR_PREFIX[] = "Error:";
const char ROOT_PREFIX[] = "[Root]";
const char NODE_PREFIX[] = "[#%d]";
const char MSG_EXIT_SIGNAL_START[] =
    "Exit signal recieved - closing all nodes...";
const char MSG_EXIT_SIGNAL_COMPLETE[] = "Bye";
const char MSG_NODE_CHECKED[] = "Node is ready for creation";
const char ERR_UNKNOWN_CMD[] = "Unknown command";
const char ERR_ALREADY_EXISTS[] = "Already exists";
const char ERR_NO_PARENT[] = "Parent not found";
const char ERR_PARENT_UNREACHABLE[] = "Parent is unavailable";
const char ERR_NOT_FOUND[] = "Not found";
const char ERR_UNAVAILABLE[] = "Node is unavailable";
const char ERR_REMOVE_ROOT[] = "Trying to remove root node - aborted";
const char ERR_HEARTBIT_RUNS[] =
    "Heartbit task already running - can`t create another";
const char ERR_SUM_BAD_ARGS[] =
    "Bad arguments: wrong number of ints or bad ints";
// * How long ping needs to wait for node to reply in seconds?
const int PING_TIMEOUT = 1;
// * How fast child should recheck thier puller for messages in seconds?
// * 0 - no delay
const int RECV_TIMEOUT = 1;
// * Command type to use in exec:
// * 1) sum
// // 2) dict
// * 3) timer
const int COMMAND_TYPE = 1;

const int BUFFER_SIZE = 1000;