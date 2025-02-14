#include "log.h"

int fd;

void initializeLog() {
    fd = open(LOG_FILE, O_RDWR | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (fd == -1) 
        errExit("Failed to open log file");
}