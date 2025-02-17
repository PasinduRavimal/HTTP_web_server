#include "log.h"

static FILE *logFile;

char *getCurrentLogFile() {
    int i = 0;
    int length = strlen(RWEBSERVER_LOG_DIR);
    struct stat fileStat;

    if (logFile != NULL)
        fclose(logFile);

    char* filename = calloc(length + 7, sizeof (char));

    if (filename == NULL)
        errExit("[WARNING] Could not allocate memory for log file name.");

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

    fflush(logFile);
}

void serverLogErrorAndExit(const char* format, ...) {
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

    va_list argList, toErrArgList;

    va_start(argList, format);
    vfprintf(logFile, formatLocal, argList);
    va_end(argList);
    
    va_start(toErrArgList, format);
    char *message = va_arg(toErrArgList, char*);
    va_end(toErrArgList);
    errExit(message);
}

static void _outputError (bool useErr, int err, bool flushStdout, const char *format, va_list ap) {
    #define BUF_SIZE 500
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, format, ap);

    if (useErr)
        snprintf(errText, BUF_SIZE, " [%s %s]", err > 0 && err <= MAX_ENAME ? ename[err] : "?UNKNOWN?", strerror(err));
    else
        snprintf(errText, BUF_SIZE, ":");

    snprintf(buf, BUF_SIZE, "[ERROR] %s %s\n", errText, userMsg);

    if (flushStdout)
        fflush(logFile);
    fputs(buf, logFile);
    fflush(logFile);
}

void serverLogError(const char *format, ...) {
    va_list argList;
    int savedErrno;

    savedErrno = errno;

    va_start(argList, format);
    _outputError(true, errno, true, format, argList);
    va_end(argList);

    errno = savedErrno;
}

void releaseLogResources() {
    fclose(logFile);
}