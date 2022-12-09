#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int create_listener(char *service) {
    struct addrinfo *res = NULL;
    int gai_err;
    struct addrinfo hint = {
            .ai_family = AF_INET,
            .ai_socktype = SOCK_STREAM,
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
        if (listen(sock, 5) < 0) {
            perror("listen");
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
        return 1;
    }
    int counter = 0;
    while (1) {
        int connection = accept(sock, NULL, NULL);
        char buf[1024];

        ssize_t all = 0;

        while (1) {
            ssize_t kolvo = read(connection, &buf, sizeof(buf));
            if (kolvo <= 0) {
                break;
            }
            all += kolvo;
            ssize_t checker = write(connection, &buf, kolvo);
            if (checker < 0) {
                _exit(0);
            }
        }
        if (all == 0) {
            ++counter;
            char path[1024];
            char *first_half = "Empty message #";

            snprintf(path, sizeof(path), "%s%d\n", first_half, counter);

            ssize_t checker = 0;
            while (strlen(path) > checker) {
                checker += write(connection, &path + checker, strlen(path));
            }
        }
        close(connection);

    }
}
