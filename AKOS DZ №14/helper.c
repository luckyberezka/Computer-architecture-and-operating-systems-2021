#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main() {
    pid_t prc;
    scanf("%d", &prc);
    int i;
    while (scanf("%d", &i) != EOF) {
        if (i == 42) {
            kill(prc, SIGTERM);
            break;
        } else {
            kill(prc, SIGRTMIN + i);
        }
    }
}
