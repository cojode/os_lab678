#ifndef CONFIG_MSG_H
#define CONFIG_MSG_H

extern const char ROOT_AWAIT_CMD_PREFIX[];
extern const char OK_PREFIX[];
extern const char ERR_PREFIX[];
extern const char ROOT_PREFIX[];
extern const char NODE_PREFIX[];
extern const char MSG_EXIT_SIGNAL_START[];
extern const char MSG_EXIT_SIGNAL_COMPLETE[];
extern const char MSG_NODE_CHECKED[];
extern const char ERR_UNKNOWN_CMD[];
extern const char ERR_ALREADY_EXISTS[];
extern const char ERR_NO_PARENT[];
extern const char ERR_PARENT_UNREACHABLE[];
extern const char ERR_NOT_FOUND[];
extern const char ERR_UNAVAILABLE[];

extern const int PING_TIMEOUT;
extern const int RECV_TIMEOUT;
extern const int BUFFER_SIZE;

#endif  // CONFIG_MSG_H