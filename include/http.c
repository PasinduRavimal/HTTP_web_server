#include "http.h"

static char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen) {
    switch (sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), s, maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), s, maxlen);
    
        default:
            strncpy(s, "Unknown AF", maxlen);
            break;
    }

    return s;
}

static char *get_port_str(const struct sockaddr *sa, char *s, size_t maxlen) {
    switch (sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_port), s, maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_port), s, maxlen);
    
        default:
            strncpy(s, "Unknown AF", maxlen);
            break;
    }

    return s;
}

void handleConnection(int sockfd, struct sockaddr *client_addr_ptr) {
    unsigned char *ptr, request[RWEBSERVER_MAX_BUF], resource[RWEBSERVER_MAX_BUF];
    char remoteIP[INET6_ADDRSTRLEN], remotePort[RWEBSERVER_PORT_MAX];
    int fd, length;

    printf("Handle Connection called");

    length = readLine(sockfd, request, RWEBSERVER_MAX_BUF);

    serverLog("Got request from %s:%d \"%s\"\n", get_ip_str(client_addr_ptr, remoteIP, INET6_ADDRSTRLEN), get_port_str(client_addr_ptr, remotePort, RWEBSERVER_PORT_MAX));

    ptr = strstr(request, " HTTP/");
    if (ptr == NULL)
        serverLog("[INFO] Unsupported request received.\n");
    else {
        *ptr = 0;
        ptr = NULL;

        if (strncmp(request, "GET ", 4) == 0)
            ptr = request + 4;
        if (strncmp(request, "HEAD ", 5) == 0)
            ptr = request + 5;

        if (ptr == NULL)
            serverLog("[INFO] Unknown request received.\n");
    }
}