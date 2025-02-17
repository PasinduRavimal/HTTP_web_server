#ifndef _RWEBSERVER_READ_LINE_H
#define _RWEBSERVER_READ_LINE_H

#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

ssize_t readLine(int, void*, size_t);

#endif