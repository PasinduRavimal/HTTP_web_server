#include "common.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define RWEBSERVER_PORT "8080"

#define RWEBSERVER_BACKLOG 10