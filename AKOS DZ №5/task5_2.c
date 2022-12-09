int mystrcmp(const char *first, const char *second) {
    int res = 0;
    unsigned char a = *first;
    unsigned char b = *second;
    while (a != '\0' || b != '\0') {
        if (b < a) {
            res = 1;
            return res;
        }
        if (a < b) {
            res = -1;
            return res;
        }
        ++first;
        a = *first;
        ++second;
        b = *second;
    }
    return res;
}
