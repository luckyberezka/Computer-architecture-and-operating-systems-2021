#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


volatile sig_atomic_t is_square = 0;

void reverse_handler(int sgn) {
    is_square = 0;
}

void square_handler(int sgn) {
    is_square = 1;
}

int main() {
    struct sigaction reverse_helper;
    reverse_helper.sa_handler = reverse_handler;
    sigaction(SIGUSR1, &reverse_helper, NULL);

    struct sigaction square_helper;
    square_helper.sa_handler = square_handler;
    sigaction(SIGUSR2, &square_helper, NULL);

    printf("%d\n", getpid());
    fflush(stdout);


    long long int number;
    while (scanf("%lld", &number) != EOF) {
        long long int res;
        if (is_square == 0) {
            res = (-1) * number;
        } else {
            res = number * number;
        }
        printf("%lld\n", res);
        fflush(stdout);
    }
}
