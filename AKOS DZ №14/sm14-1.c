#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


volatile sig_atomic_t flag = 0;

void handler(int sgn, siginfo_t *si, void *unused) {
    char number[] = "0\n";
    number[0] += flag;
    if (flag <= 3) {
        long int iter = 0;
        while (iter != strlen(number)) {
            iter += write(STDOUT_FILENO, number, strlen(number));
        }
        ++flag;
    } else {
        exit(0);
    }
}

int main() {
    struct sigaction helper;
    helper.sa_flags = SA_SIGINFO | SA_NODEFER;
    helper.sa_sigaction = handler;
    sigaction(SIGINT, &helper, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        pause();
    }
}
