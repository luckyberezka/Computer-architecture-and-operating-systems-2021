#include <stdio.h>
#include <stdbool.h>

union Helper {
    float original;
    unsigned int replica;
};

FPClass fpclassf(float value, int *psign) {
    union Helper new_val;
    new_val.original = value;

    bool flag_for_m_all_zero = true;
    for (int i = 0; i < 23; ++i) {
        if (new_val.replica % 2 == 1) {
            flag_for_m_all_zero = false;
        }
        new_val.replica = new_val.replica >> 1;
    }
    bool flag_for_exp_all_zero = true;
    bool flag_for_exp_all_one = true;
    for (int i = 0; i < 8; ++i) {
        if (new_val.replica % 2 == 1) {
            flag_for_exp_all_zero = false;
        } else {
            flag_for_exp_all_one = false;
        }
        new_val.replica = new_val.replica >> 1;
    }

    *psign = (int) (new_val.replica % 2);

    if (flag_for_exp_all_one) {
        if (flag_for_m_all_zero) {
            return FFP_INF;
        } else {
            *psign = 0;
            return FFP_NAN;
        }
    } else if (flag_for_exp_all_zero) {
        if (flag_for_m_all_zero) {
            return FFP_ZERO;
        } else {
            return FFP_DENORMALIZED;
        }
    } else {
        return FFP_NORMALIZED;
    }
}
