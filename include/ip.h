#include "common.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#ifndef _RWEBSERVER_IP_H
#define _RWEBSERVER_IP_H

typedef struct clientInfo{

    char *client_ip;
    int client_port;
    
} client_info;

client_info *get_client_info(int sockfd, const struct sockaddr *sa);
void freeClientInfo(client_info *);

#endif