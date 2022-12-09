#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        return 1;
    }
    int fd[2];
    if (pipe(fd)) {
        perror("pipec!");
        return 1;
    }
    pid_t first_son = fork();
    if (first_son == -1) {
        perror("fork failed!");
        return 1;
    }
    if (first_son == 0) {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        close(fd[0]);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }

    pid_t second_son = fork();
    if (second_son == -1) {
        perror("fork failed!");
        return 1;
    }
    if (second_son == 0) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    wait(0);
    wait(0);
    return 0;
}
