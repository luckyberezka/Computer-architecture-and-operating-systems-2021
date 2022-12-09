#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

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
        if (listen(sock, 1) < 0) {
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

int main(int argc, char* argv[]) {
    int sock = create_listener(argv[1]);
    if (sock < 0) {
        return 1;
    }
    long long int sum = 0;
    while (1) {
        int connection = accept(sock, NULL, NULL);
        int numb;
        if (read(connection, &numb, sizeof(numb)) < 0) {
            break;
        }
        numb = ntohl(numb);
        sum += numb;
        close(connection);
        if (numb == 0) {
            break;
        }
    }
    printf("%lld\n", sum);
    fflush(stdout);
    close(sock);
}
