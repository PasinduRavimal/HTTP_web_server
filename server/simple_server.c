#include "../include/common.h"
#include "../include/log.h"
#include "../include/server.h"

static void cleanup(void) __attribute__((destructor));

void sigchld_handler(int s) {
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main() {
    printf("Initializing Logs\n");
    initializeLog();

    serverLog("%s\n", "[DEBUG] Logs initialized.");
    
    int sockfd, new_fd;
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr;
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    serverLogError("[ERROR] %s\n", "This is another test error.");

    if ((rv = getaddrinfo(NULL, RWEBSERVER_PORT, &hints, &servinfo)) != 0) {
        return 1;
    }
}

void cleanup() {
    printf("Releasing Resources");
    releaseLogResources();
}