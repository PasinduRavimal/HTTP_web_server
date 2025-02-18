#include "http.h"

void handleConnection(int sockfd, struct sockaddr *client_addr_ptr) {
    unsigned char *ptr, request[RWEBSERVER_MAX_BUF], resource[RWEBSERVER_MAX_BUF];
    client_info *clinfo = get_client_info(sockfd, client_addr_ptr);
    int fd, length;

    length = readLine(sockfd, request, RWEBSERVER_MAX_BUF);

    serverLog("[INFO] Got a request from %s:%d\n", clinfo->client_ip, clinfo->client_port);

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
            serverLog("[INFO] Openning \'%s\'\n", resource);

            if (fd == -1) {
                serverLog("[INFO] 404 Not Found\n");
                writeString(sockfd, "HTTP/1.0 404 NOT FOUND\r\n");
                writeString(sockfd, "Server: R Webserver\r\n\r\n");
                writeString(sockfd, "<html><head><title>404 Not Found</title></head>");
                writeString(sockfd, "<body><h1>URL not found</h1></body></html>\r\n");
            } else {
                serverLog("[INFO] 200 OK\n");
                writeString(sockfd, "HTTP/1.0 200 OK\r\n");
                writeString(sockfd, "Server: R Webserver\r\n\r\n");
                if (ptr == request + 4) {
                    if ((length = get_file_size(fd)) == -1)
                        serverLogError("[ERROR] Getting resource file size\n");
                    if ((ptr = (unsigned char *) malloc(length)) == NULL)
                        serverLogError("[ERROR] Allocating memory for reading resource\n");
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