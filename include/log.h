#ifndef __RWEBSERVER_LOG_H
#define __RWEBSERVER_LOG_H

#include "core.h"
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>
#include "error_functions.h"
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"

#define LOG_INFO "INFO"
#define LOG_WARNING "WARNING"
#define LOG_ERROR "ERROR"

#ifndef RWEBSERVER_LOG_DIR
#define RWEBSERVER_LOG_DIR "./config"
#endif

void initializeLog();
void serverLog(const char* format, ...);
void releaseLogResources();

#endif