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