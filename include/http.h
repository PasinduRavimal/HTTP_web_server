#ifndef _RWEBSERVER_HTTP_H
#define _RWEBSERVER_HTTP_H

#include "common.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define RWEBSERVER_MAX_BUF 4096
#define RWEBSERVER_PORT_MAX 6

void handleConnection(int, struct sockaddr *);
int get_file_size(int);

#endif