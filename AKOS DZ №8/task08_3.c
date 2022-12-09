#include <stdio.h>

int GiveMeAnswer(unsigned int number) {
    unsigned i = 1;
    unsigned border = 16777216; // 2 ** 24
    for (; border != 0; border *= 2, i *= 2) {
        if (number <= border) {
            if (number % i == 0) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    if (number % 256 == 0) {
        return 1;
    } else {
        return 0;
    }
}

int main() {
    unsigned int num;
    while (scanf("%u", &num) != EOF) {
        printf("%d\n", GiveMeAnswer(num));
    }
}
