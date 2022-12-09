#include <unistd.h>
#include <wait.h>
#include <stdlib.h>

int mysys(const char *str) {
    pid_t kid = fork();
    if (kid == -1) {
        return -1;
    }
    if (kid == 0) {
        execlp("/bin/sh", "/bin/sh", "-c", str, NULL);
        _exit(127);
    }
    int status;
    if ((waitpid(kid, &status, 0)) == -1) {
        return 0;
    }
    if (WEXITSTATUS(status) >= 0 && WEXITSTATUS(status) <= 127) {
        return WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status)) {
        return (128 + WTERMSIG(status));
    }
    return 0;
}
