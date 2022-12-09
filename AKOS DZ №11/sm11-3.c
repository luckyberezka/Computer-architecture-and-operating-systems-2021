#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main () {
    pid_t first_pid;
    if (!(first_pid = fork())) {
        pid_t second_pid;
        if (!(second_pid = fork())) {
            printf("3 ");
        }
        if (second_pid != 0) {
            wait(0);
            printf("2 ");
        }
    }
    if (first_pid != 0) {
        wait(0);
        printf("1\n");
    }
}
