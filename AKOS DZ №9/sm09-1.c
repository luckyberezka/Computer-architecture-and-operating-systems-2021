#include <stdio.h>

int main() {
    unsigned int c, d, res;
    unsigned int a = 1;
    asm ( "mov %3, %%eax\n\t"
          "cpuid\n\t"
          "mov %%ecx, %0\n\t"
          "mov %%edx, %1\n\t"
          "mov %%eax, %2"
    : "=m"(c), "=m"(d), "=m"(res)
    : "m"(a)
    : "eax", "ebx", "ecx", "edx");
    unsigned int fam_id = (res << 20) >> 28;
    unsigned int fact_fam = fam_id;
    if (fam_id == 15) {
        fact_fam += (res << 4) >> 24;
    }
    unsigned int fact_model = (res << 24) >> 28;;
    if (fam_id == 6 || fam_id == 15) {
        fact_model += ((res << 12) >> 28) << 4;
    }
    printf("family=%u ", fact_fam);
    printf("model=%u ", fact_model);
    printf("ecx=0x%x ", c);
    printf("edx=0x%x\n", d);
}
