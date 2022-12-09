#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <malloc.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        return 0;
    }
    bool system_down = false;
    int prev_reader = -1;
    int fd[2];
    pid_t pids[argc - 1];
    for (int i = 1; i < argc; ++i) {
        if (i + 1 != argc) {
            if (pipe(fd)) {
                system_down = true;
                break;
            }
        }
        pid_t son = fork();
        if (son != 0) {
            pids[i - 1] = son;
        }
        if (son == -1) {
            system_down = true;
            break;
        }
        if (son == 0) {
            if (i != 1) {
                dup2(prev_reader, STDIN_FILENO);
                close(prev_reader);
            }
            if (i + 1 != argc) {
                dup2(fd[1], STDOUT_FILENO);
                close(fd[1]);
                close(fd[0]);
            }
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }


        if (i != 1) {
            close(prev_reader);
        }
        if (i + 1 != argc) {
            prev_reader = fd[0];
            close(fd[1]);
        }
    }
    for (int i = 0; i < argc - 1; ++i) {
        if (pids[i] == -1) {
            break;
        } else {
            if (system_down) {
                kill(pids[i], SIGKILL);
            }
            int status;
            waitpid(pids[i], &status, 0);
        }
    }
    if (system_down) {
        return 1;
    }
    return 0;
}
