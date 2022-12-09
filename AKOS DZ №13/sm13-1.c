#include <time.h>
#include <stdio.h>

enum {
    SecInDay = 86400,
    MonDelta = 1,
    YearDelta = 1900
};

int main() {
    long int arg;
    long int now_time = time(NULL);
    while (scanf("%ld", &arg) != EOF) {
        long int delta;
        if (__builtin_mul_overflow(arg, SecInDay, &delta)) {
            printf("OVERFLOW\n");
            continue;
        }
        long int res;

        if (__builtin_add_overflow(now_time, delta, &res)) {
            printf("OVERFLOW\n");
            continue;
        }
        

        struct tm *helper = localtime(&res);
        printf("%04d-%02d-%02d\n", helper->tm_year + YearDelta, helper->tm_mon + MonDelta, helper->tm_mday);
    }
}
