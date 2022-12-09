#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        perror("Bad arguments");
        return 0;
    }
    pid_t kid = fork();
    if (kid == 0) {
        int fd_in = open(argv[2], O_RDONLY | O_CLOEXEC);
        if (fd_in == -1) {
            perror("Bad arguments");
            exit(1);
        }
        dup2(fd_in, STDIN_FILENO);

        int fd_out = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC,
                          S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
        if (fd_out == -1) {
            perror("Bad arguments");
            exit(1);
        }
        dup2(fd_out, STDOUT_FILENO);

        int kto = execlp(argv[1], argv[1], NULL);
        if (kto) {
            exit(1);
        }
        return 127;
    }
    int status;
    if (kid != -1) {
        waitpid(kid, &status, 0);
    }
    return 0;
}
