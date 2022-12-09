#include <stdint.h>

int32_t satsum(int32_t v1, int32_t v2) {
    int32_t result;
    if (!__builtin_add_overflow(v1, v2, &result)) {
        return result;
    }
    uint32_t helper = 0 - 1;
    if (v1 < 0 || v2 < 0) {
        helper = ~(helper >> 1);
    } else {
        helper = helper >> 1;
    };
    return helper;
}

