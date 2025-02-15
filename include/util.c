#include "util.h"
 
char *intToString(int number) {
    static char string[10] = {'\0'};

    snprintf(string, 10, "%d", number);

    printf(string);

    return string;
}
