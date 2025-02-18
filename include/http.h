#include "common.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "ip.h"
#include "util.h"

#ifndef _RWEBSERVER_HTTP_H
#define _RWEBSERVER_HTTP_H

#define RWEBSERVER_MAX_BUF 4096
#define RWEBSERVER_PORT_MAX 6
#define RWEBSERVER_WEBROOT "./www"

void handleConnection(int, struct sockaddr *);

#endif