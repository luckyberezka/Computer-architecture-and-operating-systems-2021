#include <stdio.h>

union Helper {
    float original;
    unsigned int replica;
};

unsigned int TakeMantis(unsigned int *num) {
    unsigned int res = 0;
    unsigned int now_two = 1;
    for (int i = 0; i < 23; ++i) {
        if (*num % 2 == 1) {
            res += now_two;
        }
        now_two *= 2;
        *num = *num >> 1;
    }
    return res;
}

unsigned int TakeExp(unsigned int *num) {
    unsigned int res = 0;
    unsigned int now_two = 1;
    for (int i = 0; i < 8; ++i) {
        if (*num % 2 == 1) {
            res += now_two;
        }
        now_two *= 2;
        *num = *num >> 1;
    }
    return res;
}

int main() {
    float number;
    while (scanf("%f", &number) != EOF) {
        union Helper getter;
        getter.original = number;

        unsigned int m = TakeMantis(&getter.replica);
        unsigned int e = TakeExp(&getter.replica);
        unsigned int b = getter.replica % 2;
        printf("%u ", b);
        printf("%u ", e);
        printf("%x\n", m);
    }
}
