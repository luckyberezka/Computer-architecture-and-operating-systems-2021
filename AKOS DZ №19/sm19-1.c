#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    long int a = strtol(argv[1], NULL, 9);
    long int b = strtol(argv[2], NULL, 9);
    long int res;
    printf("%d\n", __builtin_add_overflow(a, b, &res));
    if (b != 0) {
        printf("%d\n", __builtin_mul_overflow(a, 1/b, &res));
    } else {
        printf("1\n");
    }
}
