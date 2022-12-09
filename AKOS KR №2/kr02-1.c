#include <stdint.h>
#include <stdio.h>

void tinyconv(uint8_t bytes[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        uint8_t bits_of_number[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        uint8_t now_number = bytes[i];
        for (size_t j = 0; j < 8; ++j) {
            if (now_number % 2 == 1) {
                bits_of_number[j] = 1;
            }
            now_number = now_number >> 1;
        }
        uint8_t res = 0;
        uint8_t now_two = 1;
        for (size_t j = 0; j < 8; ++j) {
            res = res << 1;
            if (bits_of_number[j] == 1) {
                res += 1;
            }
            now_two *= 2;
        }
        bytes[i] = res;
    }
}

