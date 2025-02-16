#include "core.h"

// char path[__RWEBSERVER_MAX_PATH_LENGTH] = {'\0'};
// const char *BASEPATH;
// const char *LOGPATH;

// void initializeFolderStructure() {
//     ssize_t count = readlink("/proc/self/exe", path, __RWEBSERVER_MAX_PATH_LENGTH - 1);

//     BASEPATH = dirname(path);
//     path[count] = "/log";
//     LOGPATH = dirname(path);

//     printf(LOGPATH);
// }