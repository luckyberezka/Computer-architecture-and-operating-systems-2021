#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <ctype.h>
#include <string.h>

int mysystem(const char *cmd) {
    if (cmd == NULL) {
        return -1;
    }
    while (isspace(*cmd) != 0) {
        ++cmd;
    }
    if (strlen(cmd) == 0) {
        return -1;
    }
    pid_t pid;
    pid = fork();
    if (pid == -1) {
        return -1;
    } else if (!pid) {
        char **buf = calloc(1, sizeof(*buf));
        char **real;
        int begin = 0;
        int cur = 0;
        int size = 0;
        int capacity = 1;
        while (cmd[begin] != '\0') {
            while (isspace(cmd[begin]) != 0) {
                ++begin;
            }
            while (!isspace(cmd[begin + cur]) && cmd[begin + cur] != '\0') {
                ++cur;
            }
            if (size == capacity) {
                capacity = capacity * 2;
                real = realloc(buf, capacity * sizeof(*buf));
                buf = real;
            }

            buf[size] = strndup(cmd + begin, cur);
            buf[size] += '\0';
            ++size;
            begin += cur;
            cur = 0;
        }
        if (size == capacity) {
            ++capacity;
            real = realloc(buf, capacity * sizeof(*buf));
            buf = real;
        }
        buf[size] = NULL;
        execvp(buf[0], buf);
        for (int i = 0; i < size; ++i) {
            free(buf[i]);
        }
        free(buf);
        _exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            return WTERMSIG(status) + 1024;
        } else if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
        return 1;
    }
}
