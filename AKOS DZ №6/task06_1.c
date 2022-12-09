#include <stdio.h>

int main(void) {
    unsigned int value_one, value_two, res, sum;
    scanf("%u", &value_one);
    scanf("%u", &value_two);
    if (value_one > value_two) {
        unsigned int helper = value_one;
        value_one = value_two;
        value_two = helper;
    }
    res = value_one;
    sum = value_one / 2;
    sum += value_two / 2;
    if (value_one % 2 == 1 && value_two % 2 == 1) {
        ++sum;
    }
    for (unsigned int i = value_one; i <= value_two; ++i) {
        if (i > sum) {
            break;
        }
        res = i;
    }
    printf("%u\n", res);
}
