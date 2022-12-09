int bitcount(STYPE value) {
    UTYPE res = 0, now = value;
    while (now > 0) {
        if (now % 2 == 1) {
            ++res;
        }
        now = now >> 1;
    }
    return res;
}
