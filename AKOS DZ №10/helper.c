#include <stdio.h>
#include <dlfcn.h>

void TakeSomeBits(int arr[12], int p) {
    unsigned int helper = (unsigned int) p;
    for (int i = 11; i >= 0; --i) {
        if (helper % 2 == 1) {
            arr[i] = 1;
        } else {
            arr[i] = 0;
        }
        helper = helper >> 1;
    }
}

const char *perms_to_str(char *buf, size_t size, int perms) {
    if (size == 0) {
        return buf;
    }
    int bits[12];
    TakeSomeBits(bits, perms);
    char full_answer[12] = "---------";
    for (int i = 3; i < 12; i += 3) {
        if (bits[i] == 1) {
            full_answer[i - 3] = 'r';
        }
        if (bits[i + 1] == 1) {
            full_answer[i + 1 - 3] = 'w';
        }
        if (bits[i + 2] == 1) {
            full_answer[i + 2 - 3] = 'x';
        }
    }
    buf[size - 1] = '\0';
    for (int i = 0; i < size - 1; ++i) {
        buf[i] = full_answer[i];
    }
    return buf;
}

int main() {
    char str[16];
    size_t sz = 16;
    int p = 640;
    perms_to_str(str, sz, p);
    printf("%s\n", str);
}

