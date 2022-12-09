#include "stdio.h"

void normalize_path(char *buf) {
    char *copy = buf;
    char *res = buf;
    int flag = 0;
    while (*copy != '\0') {
        if (*copy == '/') {
            if (flag == 0) {
                flag = 1;
                *buf = *copy;
                ++buf;
            }
        } else {
            flag = 0;
            *buf = *copy;
            ++buf;
        }
        ++copy;
    }
    *buf = '\0';
    buf = res;
}

