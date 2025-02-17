#include "../include/common.h"
#include "../include/log.h"
#include "../include/server.h"
#include "../include/inet_sockets.h"
#include "../include/http.h"
#include "../include/util.h"
#include <sys/wait.h>
#include <signal.h>

#define SERVICE "8080"
#define BUF_SIZE 4096

static void cleanup(void) __attribute__((destructor));

static void grimReaper(int sig) {
    int savedErrno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0);
    
    errno = savedErrno;
}

static void handleRequest(int cfd) {
    char buf[BUF_SIZE];
    ssize_t numRead;

    while ((numRead = read(cfd, buf, BUF_SIZE)) > 0) {
        if (write(cfd, buf, numRead) != numRead) {
            serverLogErrorAndExit("write() failed: %s", strerror(errno));
        }
    }

    if (numRead == -1) {
        serverLogErrorAndExit("Error from read(): %s", strerror(errno));
    }
}

int main() {
    printf("Initializing Logs\n");
    initializeLog();

    serverLog("%s\n", "[DEBUG] Logs initialized.");
    
    int lfd, cfd;
    struct sigaction sa;
    struct sockaddr_storage remoteaddr;
    socklen_t addrlen;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = grimReaper;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) 
        serverLogErrorAndExit("[ERROR] Error from sigaction(): %s\n", strerror(errno));

    lfd = inetListen(SERVICE, 10, NULL);
    if (lfd == -1) 
        serverLogErrorAndExit("[ERROR] Could not create server socket (%s)\n", strerror(errno));

    while (true) {
        addrlen = sizeof remoteaddr;
        cfd = accept(lfd, (struct sockaddr *)&remoteaddr, &addrlen);
        if (cfd == -1)
            serverLogErrorAndExit("[ERROR] Failure in accept(): %s", strerror(errno));

        switch (fork()) {
            case -1:
                serverLogError("Can't create child");
                close(cfd);
                break;

            case 0:
                close(lfd);
                handleConnection(cfd, (struct sockaddr *)&remoteaddr);
                _exit(EXIT_SUCCESS);
        
            default:
                close(cfd);
                break;
        }
    }
}

void cleanup() {
    printf("Releasing Resources\n");
    releaseLogResources();
}