#include "ip.h"

static char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen) {
    switch (sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), s, maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), s, maxlen);
    
        default:
            strncpy(s, "Unknown AF", maxlen);
            break;
    }

    return s;
}

client_info *get_client_info(int sockfd, const struct sockaddr *sa) {
    socklen_t length;
    struct sockaddr_storage addr;
    char ipstr[INET6_ADDRSTRLEN];
    int port;

    static client_info *ci;
    ci = calloc(1, sizeof (struct clientInfo));

    length = sizeof addr;
    if (getpeername(sockfd, (struct sockaddr *)&addr, &length) == -1) {
        serverLog("[WARNING] Could not get the peername\n");
    }

    if (addr.ss_family == AF_INET) {
        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
        port = ntohs(s->sin_port);
        inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
    } else {
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
        port = ntohs(s->sin6_port);
        inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
    }

    ci->client_ip = ipstr;
    ci->client_port = port;

    return ci;
}

