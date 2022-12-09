#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main() {
    int fd[2];

    if (pipe(fd)) {
        perror("pipec!");
        return 1;
    }

    pid_t son = fork();
    if (son == -1) {
        perror("fork failed!");
        return 1;
    }
    if (son == 0) {
        pid_t grandson = fork();
        if (grandson == -1) {
            perror("fork failed!");
            _exit(1);
        }
        if (grandson == 0) {
            long long int sum = 0;
            long int slag;
            close(fd[1]);
            while (read(fd[0], &slag, sizeof(slag))) {
                sum += slag;
            }
            printf("%lld\n", sum);
            fflush(stdout);
            _exit(0);
        }
        close(fd[1]);
        wait(0);
        _exit(0);
    }
    long int now_arg;
    while ((scanf("%ld", &now_arg)) != EOF) {
        write(fd[1], &now_arg, sizeof(now_arg));
    }
    close(fd[1]);
    wait(0);
}
