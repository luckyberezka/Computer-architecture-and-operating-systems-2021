#include <stdlib.h>
#include <stdint-gcc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

uint8_t frobnicate(uint8_t a, uint8_t b);

void Helper(int k, int N) {
    uint8_t res1, res2;
    if (k % 2 != 0 || k == 2 * N - 1 || k == 2 * N) {
        exit(k);
    }
    pid_t first_son = fork();
    if (first_son == 0) {
        Helper(k + 1, N);
    }
    pid_t second_son = fork();
    if (second_son == 0 && first_son != 0) {
        Helper(k + 2, N);
    }
    int first_flag;
    waitpid(first_son, &first_flag, 0);
    res1 = WEXITSTATUS(first_flag);

    int second_flag;
    waitpid(second_son, &second_flag, 0);
    res2 = WEXITSTATUS(second_flag);

    exit(frobnicate(res1, res2));
}

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);

    uint8_t res1, res2;

    pid_t first_son = fork();
    if (first_son == 0) {
        Helper(1, N);
    }
    pid_t second_son = fork();
    if (second_son == 0 && first_son != 0) {
        Helper(2, N);
    }
    int first_flag;
    waitpid(first_son, &first_flag, 0);
    res1 = WEXITSTATUS(first_flag);

    int second_flag;
    waitpid(second_son, &second_flag, 0);
    res2 = WEXITSTATUS(second_flag);

    printf("%d", frobnicate(res1, res2));
}
