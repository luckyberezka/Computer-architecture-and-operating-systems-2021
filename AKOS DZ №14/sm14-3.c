#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/un.h>

volatile sig_atomic_t oneusrcounter = 0;
volatile sig_atomic_t twousrcounter = 0;
volatile sig_atomic_t condition = 0;

void FirstHandler(int sig) {
    condition = 1;
}

void SecondHandler(int sig) {
    condition = 2;
}

void ThirdHandler(int sig) {
    condition = 3;
}


int main() {
    sigset_t set1, set2;
    sigemptyset(&set1);
    sigemptyset(&set2);
    sigaddset(&set1,SIGUSR1);
    sigaddset(&set1,SIGUSR2);
    sigaddset(&set1,SIGTERM);

    sigprocmask(SIG_BLOCK, &set1, NULL);
    struct sigaction one_catch = {};
    one_catch.sa_flags = SA_RESTART;
    one_catch.sa_handler = FirstHandler;
    sigaction(SIGUSR1, &one_catch, NULL);

    struct sigaction two_catch = {};
    two_catch.sa_flags = SA_RESTART;
    two_catch.sa_handler = SecondHandler;
    sigaction(SIGUSR2, &two_catch, NULL);

    struct sigaction three_catch = {};
    three_catch.sa_flags = SA_RESTART;
    three_catch.sa_handler = ThirdHandler;
    sigaction(SIGTERM, &three_catch, NULL);

    printf("%d\n", getpid());
    fflush(stdout);

    while (1) {
        sigsuspend(&set2);
        if (condition == 3) {
            return 0;
        } else if (condition == 2) {
            ++twousrcounter;
        } else if (condition == 1) {
            printf("%d %d\n", oneusrcounter, twousrcounter);
            fflush(stdout);
            ++oneusrcounter;
        }
    }
}
