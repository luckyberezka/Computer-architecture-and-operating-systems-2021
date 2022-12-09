#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    char host[1001];
    char service[1001];
    while (scanf("%1000s %1000s", host, service) != EOF) {
        struct addrinfo *res = NULL;
        int gai_err = getaddrinfo(host, service, NULL, &res);
        if (gai_err) {
            printf("%s\n", gai_strerror(gai_err));
            continue;
        }
        struct sockaddr_in *minimal = NULL;
        for (struct addrinfo *ai = res; ai; ai = ai->ai_next) {
            if (ai->ai_family == AF_INET) {
                struct sockaddr_in *helper = (struct sockaddr_in *) ai->ai_addr;
                if (minimal == NULL || ntohl(helper->sin_addr.s_addr) < ntohl(minimal->sin_addr.s_addr)) {
                    minimal = helper;
                }
            }
        }
        if (minimal != NULL) {
            printf("%s:%d\n", inet_ntoa(minimal->sin_addr), ntohs(minimal->sin_port));
        }
        freeaddrinfo(res);
    }
}
