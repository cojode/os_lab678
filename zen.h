#ifndef ZEN_H
#define ZEN_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "commands/cmds.h"
#include "struct/pool.h"

const int MAX_CMD_LENGTH = 100;

const char *OK_PREFIX = "Ok:";
const char *ERR_PREFIX = "Error:";

#endif  // ZEN_H