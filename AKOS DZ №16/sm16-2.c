#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <arpa/inet.h>

int create_listener(char* service) {
    struct addrinfo *res = NULL;
    int gai_err;
    struct addrinfo hint = {
            .ai_family = AF_INET6,
            .ai_socktype = SOCK_DGRAM,
            .ai_flags = AI_PASSIVE,
    };
    gai_err = getaddrinfo(NULL, service, &hint, &res);
    if (gai_err) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(gai_err));
        return -1;
    }
    int sock = -1;
    for (struct addrinfo *ai = res; ai; ai = ai->ai_next) {
        sock = socket(ai->ai_family, ai->ai_socktype, 0);
        if (sock < 0) {
            perror("socket");
            continue;
        }
        int one = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &one, sizeof(one))) {
            perror("setsockopt");
        }
        if (bind(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            perror("bind");
            close(sock);
            sock = -1;
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    return sock;
}

void Handler(int sig) {
    if (sig == SIGTERM) {
        _exit(0);
    }
}

int main(int argc, char *argv[]) {
    struct sigaction one_catch = {};
    one_catch.sa_handler = Handler;
    sigaction(SIGTERM, &one_catch, NULL);


    int sock = create_listener(argv[1]);
    if (sock < 0) {
        return 0;
    }
    while (1) {
        char buffer[1024];
        char helper[1024];
        struct sockaddr_in6 from = {};

        socklen_t aboba = sizeof(from);

        int how_much = recvfrom(sock, buffer, 1024, 0, (struct sockaddr *)&from, &aboba);
        buffer[how_much] = '\0';
        

        printf("[%s:%d] %s\n", inet_ntop(from.sin6_family, &from.sin6_addr, helper, sizeof(helper)),
                                htons(from.sin6_port),
                                buffer);
        fflush(stdout);
    }
}
