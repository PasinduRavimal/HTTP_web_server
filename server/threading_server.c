#include "../include/common.h"
#include "../include/log.h"
#include "../include/server.h"
#include "../include/inet_sockets.h"
#include "../include/http.h"
#include "../include/util.h"
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>

#define SERVICE "8080"
#define BUF_SIZE 4096

struct sockaddr_storage remoteaddr;
socklen_t addrlen;

static void cleanup(void) __attribute__((destructor));

void *doit(void *arg) {
    pthread_detach(pthread_self());
    handleConnection((int) arg, (struct sockaddr *)&remoteaddr);
    close((int) arg);
    return (NULL);
}

int main() {
    printf("Initializing Logs\n");
    initializeLog();

    serverLog("%s\n", "[DEBUG] Logs initialized.");
    
    int lfd, cfd;
    pthread_t tid;
    char s[INET6_ADDRSTRLEN];

    // if (becomeDaemon(0) == -1)
    //     serverLogErrorAndExit("becomeDaemon\n");

    lfd = inetListen(SERVICE, 10, NULL);
    if (lfd == -1) 
        serverLogErrorAndExit("[ERROR] Could not create server socket (%s)\n", strerror(errno));

    serverLog("[INFO] Waiting for connections...\n");
    
    while (true) {
        addrlen = sizeof remoteaddr;
        cfd = accept(lfd, (struct sockaddr *)&remoteaddr, &addrlen);
        if (cfd == -1)
            serverLogErrorAndExit("[ERROR] Failure in accept(): %s\n", strerror(errno));

        pthread_create(&tid, NULL, &doit, (void *) cfd);
    }
}

void cleanup() {
    printf("Releasing Resources\n");
    releaseLogResources();
}