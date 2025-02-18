#ifndef _RWEBSERVER_READ_LINE_H
#define _RWEBSERVER_READ_LINE_H

#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define BD_NO_CHDIR 01
#define BD_NO_CLOSE_FILES 02
#define BD_NO_REOPEN_STD_FDS 04

#define BD_NO_UMASK0 010

#define BD_MAX_CLOSE 8192

char *intToString(int number);
ssize_t readLine(int, void*, size_t);
int becomeDaemon(int flags);
ssize_t readn(int fd, void *buffer, size_t count);
ssize_t writen(int fd, void *buffer, size_t count);
ssize_t writeString(int fd, void *buffer);
int get_file_size(int);

#endif