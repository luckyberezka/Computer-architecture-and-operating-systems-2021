#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <stdlib.h>

int Executor(char *cmd) {
    pid_t frk = fork();
    if (frk == -1) {
        return 1;
    }
    if (frk == 0) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    }
    int status;
    waitpid(frk, &status, 0);
    if (WIFEXITED(status)) {
        if (WEXITSTATUS(status) == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if ((Executor(argv[1]) == 0 || Executor(argv[2]) == 0) && Executor(argv[3]) == 0) {
        return 0;
    }
    return 1;
}
