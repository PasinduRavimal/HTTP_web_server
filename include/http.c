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

    serverLog("[DEBUG] Handle Connection called\n");

    length = readLine(sockfd, request, RWEBSERVER_MAX_BUF);

    serverLog("[DEBUG] Back again in http.c\n");

    // serverLog("[INFO] Got request from %s:%d \"%s\"\n", get_ip_str(client_addr_ptr, remoteIP, INET6_ADDRSTRLEN), get_port_str(client_addr_ptr, remotePort, RWEBSERVER_PORT_MAX));

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
        else {
            if (ptr[strlen(ptr) - 1] == '/')
                strcat(ptr, "index.html");
            strncpy(resource, RWEBSERVER_WEBROOT, RWEBSERVER_MAX_BUF);
            strncat(resource, ptr, RWEBSERVER_MAX_BUF - strlen(resource));

            fd = open(resource, O_RDONLY, 0);
            serverLog("[INFO] Openning \'%s\'\t", resource);

            if (fd == -1) {
                serverLog(" 404 Not Found\n");
                writeString(sockfd, "HTTP/1.0 404 NOT FOUND\r\n");
                writeString(sockfd, "Server: R Webserver\r\n\r\n");
                writeString(sockfd, "<html><head><title>404 Not Found</title></head>");
                writeString(sockfd, "<body><h1>URL not found</h1></body></html>\r\n");
            } else {
                serverLog(" 200 OK\n");
                writeString(sockfd, "HTTP/1.0 OK\r\n");
                writeString(sockfd, "Server: R Webserver\r\n\r\n");
                if (ptr == request + 4) {
                    if ((length = get_file_size(fd)) == -1)
                        serverLogError("[ERROR] Getting resource file size\n");
                    if ((ptr = (unsigned char *) malloc(length)) == NULL)
                        serverLogError("[ERROR Allocatign memory for reading resource\n");
                    read(fd, ptr, length);
                    writen(sockfd, ptr, length);
                    free(ptr);
                    ptr = NULL;
                }
                close(fd);
            }
        }
    }

    shutdown(sockfd, SHUT_RDWR);
}

int get_file_size(int fd) {
    struct stat stat_struct;

    if (fstat(fd, &stat_struct) == -1)
        return -1;
    
    return (int)stat_struct.st_size;
}