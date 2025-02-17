#include "read_line.h"

ssize_t readLine(int fd, void *buffer, size_t n) {
    ssize_t numRead;
    size_t totRead;
    char *buf;
    char ch;

    if (n <= 0 || buffer == NULL) {
        errno = EINVAL;
        return -1;
    }

    buf = buffer;

    totRead = 0;
    while (true) {
        numRead = read(fd, &ch, 1);

        if (numRead)
    }
}