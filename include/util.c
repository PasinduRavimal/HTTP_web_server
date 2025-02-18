#include "util.h"
#include <unistd.h>
#include "../include/log.h"
 
char *intToString(int number) {
    static char string[10] = {'\0'};

    snprintf(string, 10, "%d", number);

    printf(string);

    return string;
}

ssize_t readLine(int fd, void *buffer, size_t n) {
    ssize_t numRead;
    size_t totRead;
    char *buf;
    char ch;

    serverLog("[DEBUG] readLine function reached.\n");

    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;

    totRead = 0;
    while (true) {
        numRead = read(fd, &ch, 1);

        if (numRead == -1) {
            if (errno == EINTR)
                continue;
            else
                return -1;
        } else if (numRead == 0) {
            if (totRead == 0)
                return 0;
            else 
                break;
        } else {
            if (totRead < n - 1) {
                totRead++;
                *buf++ = ch;
            }

            if (ch == '\n')
                break;
        }
    }

    *buf = '\0';
    serverLog("[DEBUG] end of readline reached!\n");
    return totRead;
}

int becomeDaemon(int flags) {
    int maxfd, fd;

    switch (fork()) {
        case -1:
            return -1;
        
        case 0:
            break;
    
        default:
            _exit(EXIT_SUCCESS);
    }

    if (setsid() == -1)
        return -1;

    switch (fork()) {
        case -1:
            return -1;

        case 0:
            break;

        default:
            _exit(EXIT_SUCCESS);
    }

    if (!(flags & BD_NO_UMASK0))
        umask(0);
    
    if (!(flags & BD_NO_CHDIR))
        chdir("/");

    if (!(flags & BD_NO_CLOSE_FILES)) {
        maxfd = sysconf(_SC_OPEN_MAX);
        if (maxfd == -1)
            maxfd = BD_MAX_CLOSE;

        for (fd = 0; fd < maxfd; fd++)
            close(fd);
    }

    if (!(flags & BD_NO_REOPEN_STD_FDS)) {
        close(STDIN_FILENO);

        fd = open("/dev/null", O_RDWR);

        if (fd != STDIN_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
            return -1;
        if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
            return -1;
    }

    return 0;
}

ssize_t readn(int fd, void *buffer, size_t n) {
    ssize_t numRead;
    size_t totRead;
    char *buf;

    buf = buffer;
    for (totRead = 0; totRead < n; ) {
        numRead = read(fd, buf, n - totRead);

        if (numRead == 0)
            return totRead;
        if (numRead == -1) {
            if (errno == EINTR)
                continue;
            else
                return -1;
        }

        totRead += numRead;
        buf += numRead;
    }

    return totRead;
}

ssize_t writen(int fd, void *buffer, size_t n) {
    ssize_t numWritten;
    size_t totWritten;
    const char *buf;

    buf = buffer;
    for (totWritten = 0; totWritten < n; ) {
        numWritten = write(fd, buf, n - totWritten);

        if (numWritten <= 0){
            if (numWritten == -1 && errno == EINTR)
                continue;
            else
                return -1;
        }

        totWritten += numWritten;
        buf += numWritten;
    }

    return totWritten;
}

ssize_t writeString(int fd, void* buffer) {
    const char *buf;
    ssize_t bufLen;

    buf = buffer;
    bufLen = strlen(buf);

    return writen(fd, buffer, bufLen);
}