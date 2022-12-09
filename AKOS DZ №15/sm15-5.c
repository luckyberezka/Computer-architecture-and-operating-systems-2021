#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int create_listener(char* service) {
    struct addrinfo *res = NULL;
    int gai_err;
    struct addrinfo hint = {
            .ai_family = AF_UNSPEC,
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
    if (sig == SIGCHLD) {
        wait(0);
    } else if (sig == SIGTERM) {
        kill(0, SIGINT);
        while (waitpid(-1, NULL, WNOHANG) > 0) {}
        _exit(0);
    } else if (sig == SIGPIPE || sig == SIGINT) {
        _exit(0);
    }
}

int main(int argc, char* argv[]) {
    struct sigaction one_catch = {};
    one_catch.sa_handler = Handler;
    sigaction(SIGCHLD, &one_catch, NULL);
    sigaction(SIGTERM, &one_catch, NULL);
    sigaction(SIGPIPE, &one_catch, NULL);
    sigaction(SIGINT, &one_catch, NULL);

    int sock = create_listener(argv[1]);
    if (sock < 0) {
        return 1;
    }
    int counter = 1;
    while (1) {
        int connection = accept(sock, NULL, NULL);
        if (connection < 0) {
            continue;
        }
        pid_t child = fork();
        if (!child) {
            FILE *rmyfd = fdopen(dup(connection), "r");
            FILE *wmyfd = fdopen(connection, "w");
            fprintf(wmyfd, "%s\r\n", argv[2]);
            fflush(wmyfd);
            fprintf(wmyfd, "%d\r\n", counter);
            fflush(wmyfd);

            int max = 0;
            if (fscanf(rmyfd, "%d", &max) < 0) {
                _exit(0);
            }
            fprintf(wmyfd, "%d\r\n", max);
            fflush(wmyfd);
            
            while (1) {
                int num = 0;
                if (fscanf(rmyfd, "%d", &num) < 0) {
                    _exit(0);
                }
                int now_sum = 0;
                if (num > max || __builtin_add_overflow(num, counter, &now_sum)) {
                    fprintf(wmyfd, "%d\r\n", -1);
                    fflush(wmyfd);
                    _exit(0);
                } else {
                    fprintf(wmyfd, "%d\r\n", now_sum);
                    fflush(wmyfd);
                }
            }
        }
        ++counter;
        close(connection);
    }
}
