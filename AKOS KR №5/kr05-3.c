#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int Executor(char *cmd, int fd_out) {
    pid_t frk = fork();
    if (frk == -1) {
        return 1;
    }
    if (frk == 0) {

        dup2(fd_out, STDOUT_FILENO);

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
    int fd_out = open(argv[3], O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, 0666);
    if (Executor(argv[1], fd_out) == 0 || Executor(argv[2], fd_out) == 0) {
        close(fd_out);
        return 0;
    }
    close(fd_out);
    return 1;
}
