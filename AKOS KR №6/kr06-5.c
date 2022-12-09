#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/signalfd.h>
#include <string.h>

enum {
    BUF_SIZE = 4096,
};

ssize_t copy_file(int in_fd, int out_fd) {
    char buf[BUF_SIZE];
    if (in_fd < 0) {
        return -1;
    }
    if (out_fd < 0) {
        return -1;
    }
    ssize_t answer = 0;
    ssize_t result;
    while ((result = read(in_fd, buf, BUF_SIZE)) > 0) {
        ssize_t now;
        ssize_t counter = 0;
        while (counter != result) {
            now = write(out_fd, buf + counter, result - counter);
            if (now < 0) {
                return -1;
            }
            counter += now;
        }
        answer += counter;
    }
    return answer;
}

void handler(int sgn) {
    char number[] = "We communicate on my terms.\n";
    long int iter = 0;
    while (iter != strlen(number)) {
        iter += write(STDOUT_FILENO, number, strlen(number));
    }
}

int main() {
    struct sigaction one_catch = {};
    one_catch.sa_flags = SA_RESTART;
    one_catch.sa_handler = handler;
    sigaction(SIGTERM, &one_catch, NULL);
    
    copy_file(STDIN_FILENO, STDOUT_FILENO);
}
