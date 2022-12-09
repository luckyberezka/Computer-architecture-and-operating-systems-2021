#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <string.h>


int interact(const char* cmd, const char* input, char* outbuf) {
    int fd0[2];
    if (pipe(fd0)) {
        perror("pipec!");
        return 1;
    }

    int fd1[2];
    if (pipe(fd1)) {
        perror("pipec!");
        return 1;
    }

    pid_t first_son = fork();
    if (first_son == -1) {
        perror("fork failed!");
        return 127;
    }
    if (first_son == 0) {
        close(fd1[0]);
        close(fd0[1]);

        dup2(fd0[0], STDIN_FILENO);
        dup2(fd1[1], STDOUT_FILENO);

        close(fd0[0]);
        close(fd1[1]);

        execlp(cmd, cmd, NULL);
        _exit(127);
    }

    write(fd0[1], input, strlen(input));

    close(fd1[1]);
    close(fd0[1]);

    ssize_t all_bytes = 0;

    while (1) {
        ssize_t now = read(fd1[0], outbuf + all_bytes, 1024);
        if (now <= 0) {
            break;
        }
        all_bytes += now;
    }

    outbuf[all_bytes] = '\0';


    close(fd0[0]);
    close(fd1[0]);
    int status;
    waitpid(first_son, &status, 0);
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return 127;
}

