#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


// взята из лекции от 22.04.2022
int create_connection(char* node, char* service) {
    struct addrinfo *res = NULL;
    int gai_err;
    struct addrinfo hint = {
            .ai_family = AF_UNSPEC,     // можно и AF_INET, и AF_INET6
            .ai_socktype = SOCK_STREAM, // но мы хотим поток (соединение)
    };
    gai_err = getaddrinfo(node, service, &hint, &res);
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
        if (connect(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            perror("connect");
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
    _exit(0);
}

int main(int argc, char* argv[]) {
    struct sigaction one_catch = {};
    one_catch.sa_handler = Handler;
    sigaction(SIGPIPE, &one_catch, NULL);

    int sock = create_connection(argv[1], argv[2]);
    if (sock < 0) {
        return 0;
    }
    FILE *chat = fdopen(sock, "a+");
    fprintf(chat, "%s\n", argv[3]);
    fflush(chat);
    unsigned long long number;
    int for_check = fscanf(chat, "%llu", &number);
    if (for_check < 0) {
        fclose(chat);
        close(sock);
        return 0;
    }
    for (unsigned long long i = 0; i <= number; ++i) {
        fprintf(chat, "%llu\n", i);
        fflush(chat);
    }
    unsigned long long last;
    for_check = fscanf(chat, "%llu", &last);
    if (for_check < 0) {
        fclose(chat);
        close(sock);
        return 0;
    }
    printf("%llu\n", last);
    fflush(stdout);
    fclose(chat);
    close(sock);
}
