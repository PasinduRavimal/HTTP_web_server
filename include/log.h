#ifndef __RWEBSERVER_LOG_H
#define __RWEBSERVER_LOG_H

#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include "error_functions.h"

#define LOG_INFO "INFO"
#define LOG_WARNING "WARNING"
#define LOG_ERROR "ERROR"

#define LOG_FILE "../log/log"

void initializeLog();

#endif