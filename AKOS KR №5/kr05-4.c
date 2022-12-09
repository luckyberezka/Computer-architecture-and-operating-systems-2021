#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int Executor1(char *cmd) {
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
    if (Executor1(argv[1])) {
        int fd_out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0666);
        if (fd_out == -1) {
            return 1;
        }
        if ((dup2(fd_out, STDOUT_FILENO)) == -1) {
            close(fd_out);
            return 1;
        }
        if (!Executor1(argv[2])) {
            return 0;
        }

        return 1;
    }
    return 0;
}
