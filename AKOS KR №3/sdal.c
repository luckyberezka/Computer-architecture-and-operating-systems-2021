#include <stdint.h>
#include <math.h>
#include <stdio.h>

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
    F_SPECIAL_EXP = 256
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

            /*mantis = mantis >> (FSIZE_OF_MANTIS - HSIZE_OF_MANTIS);
            uint16_t mantis_for_res = mantis;
            res |= mantis_for_res;*/

            f *= (1 << HSIZE_OF_MANTIS);
            f = roundf(f * 0.5f) * 2.0f;
            f /= (1 << HSIZE_OF_MANTIS);

            union AnotherView all_right;
            all_right.real = f;

            mantis = all_right.copy << (FSIZE_OF_SIGN + FSIZE_OF_EXP);
            mantis = mantis >> (FSIZE_OF_SIGN + FSIZE_OF_EXP);
            uint16_t mantis_for_answer = mantis;

            res |= mantis_for_answer;
        }
    }
    return res;
}


#include <ctype.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

uint16_t generate_res(uint16_t sign, uint16_t exp, uint16_t m) {
    uint16_t res = sign;
    res <<= 15;
    res += (exp << 10);
    res += m;
    return res;
}
uint16_t float_to_fp162(float f) {
    uint32_t sign, exponent, fraction;
    uint16_t sign_res, exp_res = 0, frac_res;
    int16_t expppp;
    union {
        float    value;
        unsigned bites;
    } Float;
    Float.value = f;
    sign = Float.bites >> (8 + 23);
    exponent = (Float.bites >> 23) & ((1 << 8) -1);
    fraction = Float.bites & ((1 << 23) - 1);
    sign_res = sign;
    if ((exponent ^ ((1 << 8) - 1)) == 0 && fraction != 0) {
        exp_res = (1 << 5) - 1;
        frac_res = 1;
        return generate_res(sign_res, exp_res, frac_res);
    }
    if ((exponent ^ ((1 << 8) - 1)) == 0 && fraction == 0) {
        exp_res = (1 << 5) - 1;
        frac_res = 0;
        return generate_res(sign_res, exp_res, frac_res);
    }

    if (exponent == 0 && fraction == 0) {
        exp_res = 0;
        frac_res = 0;
        return generate_res(sign_res, exp_res, frac_res);
    }
    uint16_t ml_m, st_ml;
    ml_m = fraction;
    ml_m <<= 3;
    ml_m >>= 3;
    st_ml = (fraction >> 13);
    if (ml_m < (1 << 12)) {
        frac_res = st_ml;
        expppp = (int)exponent - 127;
        if ( -14 <= expppp && expppp <= 15) {
            exp_res = (uint16_t) expppp;
            exp_res += 15;
            return generate_res(sign_res, exp_res, frac_res);
        } else {
            exp_res = (1 << 5) - 1;
            frac_res = 0;
            return generate_res(sign_res, exp_res, frac_res);
        }
    } else {
        if (ml_m > (1 << 12)) {
            if (st_ml == (1 << 10) - 1) {
                ++exp_res;
                st_ml = 0;
            } else {
                st_ml += 1;
            }
        } else {
            if (st_ml % 2 == 1) {
                if (st_ml == (1 << 10) - 1) {
                    ++exp_res;
                    st_ml = 0;
                } else {
                    st_ml += 1;
                }
            }
        }
    }
    expppp = (int16_t) (exponent) - 127;
    expppp += (int16_t) (exp_res);
    if ( -14 <= expppp && expppp <= 15) {
        frac_res = st_ml;
        exp_res = (uint16_t) expppp;
        exp_res += 15;
        return generate_res(sign_res, exp_res, frac_res);
    } else {
        exp_res = (1 << 5) - 1;
        frac_res = 0;
        return generate_res(sign_res, exp_res, frac_res);
    }
}

int main() {
    for (uint32_t i = 0; i < UINT32_MAX; ++i) {
        union AnotherView h;
        h.copy = i;
        if (float_to_fp16(h.real) != float_to_fp162(h.real)) {
            printf("%u\n", i);
        }
    }
}
