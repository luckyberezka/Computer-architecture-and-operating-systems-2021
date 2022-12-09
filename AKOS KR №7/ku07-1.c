#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdbool.h>

int Helper(const char *str) {
    int for_ret = 0;
    int i = 0;
    int now_numb = 0;
    while (str[i] != '\0') {
        if (str[i] == '.') {
            for_ret *= 256;
            for_ret += now_numb;
            now_numb = 0;
        } else {
            now_numb *= 10;
            now_numb += str[i] - '0';
        }
        ++i;
    }
    for_ret *= 256;
    for_ret += now_numb;
    return for_ret;
}

bool is_ipv4_subnet(const char* mask, const char* subnet_address, const char* address) {

    int msk = Helper(mask);
    int sbnt = Helper(subnet_address);
    int adr = Helper(address);
    bool answ = true;
    if ((adr & msk) != (sbnt & msk)) {
        answ = false;
    }
    return answ;
}
