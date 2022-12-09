#include <stdio.h>

union Helper {
    float f;
    unsigned int u;
};

void myfloorf(float *arg) {
    union Helper number;
    number.f = *arg;
    unsigned int true_exp = number.u >> 23;
    if (true_exp < 127) {
        number.u = 0;
        *arg = number.f;
        return;
    }
    if (true_exp < 150) {
        unsigned int delta = 23 - (true_exp - 127);
        number.u = number.u >> delta;
        number.u = number.u << delta;
        *arg = number.f;
        return;
    }
}
