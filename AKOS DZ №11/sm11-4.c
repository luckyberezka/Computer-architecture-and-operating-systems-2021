#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void DoSmth() {
    int number;
    if (scanf("%d", &number) == EOF) {
        exit(0);
    }
    pid_t now_frk;
    if (!(now_frk = fork())) {
        DoSmth();
    } else if (now_frk == -1) {
        exit(1);
    }

    int status;
    waitpid(now_frk, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        printf("%d\n", number);
        exit(0);
    } else {
        exit(1);
    }
}

int main() {
    pid_t my_fork;
    if (!(my_fork = fork())) {
        DoSmth();
    } else if (my_fork == -1) {
        printf("%d\n", -1);
        return 0;
    }
    int status;
    waitpid(my_fork, &status, 0);
    if (WIFEXITED(status) && WEXITSTATUS(status) != 0) {
        printf("%d\n", -1);
    }
    return 0;
}
