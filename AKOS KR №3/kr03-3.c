#include <stdio.h>

void print_parent_arg() {
    int res_arg, a;
    asm ( "mov (%%ebp), %%eax\n\t"
          "mov 8(%%eax), %%ecx\n\t"
          "mov %%ecx, %0"
    : "=m"(res_arg)
    : "m"(a)
    : "eax", "ebx", "ecx", "edx");
    printf("%x\n", res_arg);
}
