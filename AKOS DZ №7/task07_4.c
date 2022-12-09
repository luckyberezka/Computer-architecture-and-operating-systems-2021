#include <stdint.h>
#include <stdio.h>

int imull_overflow(int64_t a, int64_t b, int64_t *res) {
    if (a == 0 || b == 0) {
        *res = 0;
        return 0;
    }
    if (a > b) {
        int64_t c = a;
        a = b;
        b = c;
    }
    if (a > 0 && b > 0) {
        if (a > INT64_MAX / b) {
            uint64_t karakatica = (uint64_t) a * (uint64_t) b;
            *res = karakatica;
            return 1;
        }
        *res = a * b;
        return 0;
    } else if (a < 0 && b < 0) {
        if (a < INT64_MAX / b) {
            uint64_t karakatica = (uint64_t) a * (uint64_t) b;
            *res = karakatica;
            return 1;
        }
        *res = a * b;
        return 0;
    } else {
        if (a < INT64_MIN / b) {
            uint64_t karakatica = (uint64_t) a * (uint64_t) b;
            *res = karakatica;
            return 1;
        }
        *res = a * b;
        return 0;
    }
}

