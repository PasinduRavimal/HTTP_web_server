#ifndef __RWEBSERVER_CORE_H
#define __RWEBSERVER_CORE_H

#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>

#ifndef __RWEBSERVER_MAX_PATH_LENGTH
#define __RWEBSERVER_MAX_PATH_LENGTH 1024
#endif

void initializeFolderStructure();

#endif