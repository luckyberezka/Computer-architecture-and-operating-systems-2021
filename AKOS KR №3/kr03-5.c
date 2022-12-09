#include <stdint.h>

enum {
    H_MAX_NORMAL = 15,

    H_BIAS = 15,

    HSIZE_OF_MANTIS = 10,
    HSIZE_OF_EXP = 5,
    HSIZE_OF_SIGN = 1,
    H_SPECIAL_EXP = 31,

    F_BIAS = 127,

    FSIZE_OF_MANTIS = 23,
    FSIZE_OF_EXP = 8,
    FSIZE_OF_SIGN = 1,
    F_SPECIAL_EXP = 255
};

union AnotherView {
    float real;
    uint32_t copy;
};

uint16_t float_to_fp16(float f) {
    uint16_t res = 0;


    union AnotherView helper;
    helper.real = f;

    uint32_t now = helper.copy;

    uint16_t sign = now >> (FSIZE_OF_EXP + FSIZE_OF_MANTIS);

    uint32_t exp = now << FSIZE_OF_SIGN;
    exp = exp >> (FSIZE_OF_SIGN + FSIZE_OF_MANTIS);

    uint32_t mantis = now << (FSIZE_OF_SIGN + FSIZE_OF_EXP);
    mantis = mantis >> (FSIZE_OF_SIGN + FSIZE_OF_EXP);


    if (exp == 0 && mantis == 0) { // zero
        res |= sign;
        res = res << (HSIZE_OF_MANTIS + HSIZE_OF_EXP);
    } else if (exp == F_SPECIAL_EXP) { // inf or nan
        res |= sign;
        res = res << (HSIZE_OF_EXP + HSIZE_OF_MANTIS);
        uint16_t exp_for_res = H_SPECIAL_EXP;
        exp_for_res = exp_for_res << HSIZE_OF_MANTIS;
        res |= exp_for_res;

        if (mantis != 0) { // nan == not inf
            res += 1;
        }
    } else { // number
        if (exp > H_MAX_NORMAL + F_BIAS) { // overflow
            res |= sign;
            res = res << (HSIZE_OF_EXP + HSIZE_OF_MANTIS);
            uint16_t exp_for_res = H_SPECIAL_EXP;
            exp_for_res = exp_for_res << HSIZE_OF_MANTIS;
            res |= exp_for_res;
        } else { // all right
            res |= sign;
            res = res << (HSIZE_OF_EXP + HSIZE_OF_MANTIS);

            exp -= F_BIAS;
            exp += H_BIAS;
            uint16_t exp_for_res = exp << HSIZE_OF_MANTIS;
            res |= exp_for_res;

            mantis = mantis >> (FSIZE_OF_MANTIS - HSIZE_OF_MANTIS);
            uint16_t mantis_for_res = mantis;
            res |= mantis_for_res;
        }
    }
    return res;
}


