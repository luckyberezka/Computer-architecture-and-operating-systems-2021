#include <stdint.h>
#include <stdbool.h>

enum {
    MAX_MANTIS = 1023,
    MAX_EXP = 30,
    SIZE_OF_MANTIS = 10,
    SIZE_OF_EXP = 5,
    SIZE_OF_SIGN = 1
};

uint16_t sat_mul4_half(uint16_t now) {
    uint16_t exp = now << SIZE_OF_SIGN;
    exp = exp >> (SIZE_OF_SIGN + SIZE_OF_MANTIS);

    uint16_t sign = now >> (SIZE_OF_EXP + SIZE_OF_MANTIS);
    sign = sign << (SIZE_OF_EXP + SIZE_OF_MANTIS);

    uint16_t mantis = now << (SIZE_OF_SIGN + SIZE_OF_EXP);
    mantis = mantis >> (SIZE_OF_SIGN + SIZE_OF_EXP);

    bool exp_all_one = false;
    bool exp_all_zero = false;

    if (exp == 0) {
        exp_all_zero = true;
    } else if (exp == MAX_EXP + 1) {
        exp_all_one = true;
    }

    if (exp_all_one || now == 0) {
        return now;
    }

    if (!exp_all_zero) {
        if (exp + 2 < MAX_EXP + 1) {
            exp += 2;
            exp = exp << SIZE_OF_MANTIS;
            sign |= exp;
            sign |= mantis;
        } else {
            exp = MAX_EXP;
            exp = exp << SIZE_OF_MANTIS;
            mantis = MAX_MANTIS;
            sign |= exp;
            sign |= mantis;
        }
    } else {
        uint16_t first_bit_of_mantis = mantis >> 9;
        if (first_bit_of_mantis == 1) {
            exp = 2;
            exp = exp << SIZE_OF_MANTIS;
            mantis = mantis << (SIZE_OF_SIGN + SIZE_OF_EXP + 1);
            mantis = mantis >> (SIZE_OF_SIGN + SIZE_OF_EXP);
            sign |= exp;
            sign |= mantis;
        } else {
            exp |= mantis;
            exp = exp << 2;
            sign |= exp;
        }
    }
    return sign;
}

