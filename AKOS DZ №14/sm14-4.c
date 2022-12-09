#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/signalfd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <errno.h>

volatile sig_atomic_t is_term = 0;
volatile sig_atomic_t is_min = 0;
volatile sig_atomic_t somebody_catch = 0;

void Handler(int sig) {
    somebody_catch = 1;
    if (sig == SIGTERM) {
        is_term = 1;
        is_min = 0;
    } else {
        is_term = 0;
        is_min = sig - SIGRTMIN + 1;
    }
}

int main(int argc, char *argv[]) {
    int fixed = argc - 1;
    sigset_t blockset, emptyset;
    sigemptyset(&blockset);
    sigemptyset(&emptyset);

    struct sigaction hnd = {};
    hnd.sa_flags = 0;
    hnd.sa_handler = Handler;
    sigaction(SIGTERM, &hnd, NULL);
    sigaddset(&blockset, SIGTERM);
    for (int i = 0; i < fixed; ++i) {
        sigaction(SIGRTMIN + i, &hnd, NULL);
        sigaddset(&blockset, SIGRTMIN + i);
    }
    sigprocmask(SIG_BLOCK, &blockset, NULL);

    long long int sums[argc];
    for (int i = 0; i < argc; ++i) {
        sums[i] = 0;
    }

    int status[argc];
    for (int i = 0; i < argc; ++i) {
        status[i] = 0;
    }

    printf("%d\n", getpid());
    fflush(stdout);

    while (1) {
        while (somebody_catch == 0) {
            sigsuspend(&emptyset);
        }
        if (is_term == 1) {
            for (int i = 1; i < argc; ++i) {
                printf("%lld\n", sums[i]);
                fflush(stdout);
            }
            exit(0);
        } else {
            if (status[is_min] == 1)  {
                somebody_catch = 0;
                continue;
            }
            FILE *fd = fopen(argv[is_min], "r");
            if (fd == NULL) {
                somebody_catch = 0;
                continue;
            }
            char buffer[16];
            while (1) {
                errno = 0;
                sigprocmask(SIG_UNBLOCK, &blockset, NULL);
                int bytes = read(fileno(fd), buffer, 16);
                sigprocmask(SIG_BLOCK, &blockset, NULL);
                if (errno == EINTR) {
                    break;
                }
                if (bytes == 0 && errno == 0) {
                    somebody_catch = 0;
                    status[is_min] = 1;
                    fclose(fd);
                    break;
                }
                sums[is_min] += strtoll(buffer, NULL, 10);
            }
        }
    }
}
