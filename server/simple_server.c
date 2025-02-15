#include "../include/common.h"
#include "../include/log.h"

static void cleanup(void) __attribute__((destructor));

int main() {
    printf("Initializing Logs\n");
    initializeLog();

    serverLog("%s\n", "[DEBUG] Logs initialized.");
    
}

void cleanup() {
    printf("Releasing Resources");
    releaseLogResources();
}