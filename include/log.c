#include "log.h"

static FILE *logFile;

char *getCurrentLogFile() {
    int i = 0;
    int length = strlen(RWEBSERVER_LOG_DIR);
    struct stat fileStat;

    char* filename = calloc(length + 7, sizeof (char));
    strncpy(filename, RWEBSERVER_LOG_DIR, length);
    strcat(filename, "/log");

    while (1) {
        snprintf(filename + length + 4, 3, "%02d", i);
        if (access(filename, F_OK) != 0) {
            if (i == 0)
                break;
            else {
                --i;
                break;
            }
        }
        i++;
    }

    snprintf(filename + length + 4, 3, "%02d", (i) % 100);

    if (stat(filename, &fileStat) == -1) {
        errMsg("[WARNING] Could not get log file statistics.");
    } else {
        if (fileStat.st_size > 2000000) {
            snprintf(filename + length + 4, 3, "%02d", (++i) % 100);
        }
    }

    return filename;
}

void freeCurrentLogFile(char *returnedPointer) {
    free(returnedPointer);
    returnedPointer = NULL;
}

void initializeLog() {
    char *filename = getCurrentLogFile();
    logFile = fopen(filename, "a");
    freeCurrentLogFile(filename);

    if (logFile == NULL) 
        errExit("Failed to open log file");
}

void serverLog(const char* format, ...) {
    time_t t = time(NULL);
    char* time = ctime(&t);
    char timeLocal[30] = {'\0'};
    char c = *time;
    int i = 0;
    char formatLocal[100];

    while(c != '\n') {
        timeLocal[i++] = c;
        c = time[i];
    }

    snprintf(formatLocal, 100, "%s %s", timeLocal, format);

    va_list argList;

    va_start(argList, format);
    vfprintf(logFile, formatLocal, argList);
    va_end(argList);
}

void releaseLogResources() {
    fclose(logFile);
}