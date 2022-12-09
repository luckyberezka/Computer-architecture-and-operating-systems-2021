#include <stdint.h>

void work_until_deadline(const uint64_t *deadline, void (*do_work)()) {
    unsigned int first;
    unsigned int second;
    unsigned int a = 0;
    asm ( "rdtsc\n\t"
          "mov %%edx, %0\n\t"
          "mov %%eax, %1\n\t"
    : "=m"(first), "=m"(second)
    : "m"(a)
    : "eax", "ebx", "ecx", "edx");
    uint64_t one_half = first, two_half = second;
    one_half = one_half << 32;
    one_half ^= two_half;
    while (one_half < *deadline) { 
        do_work();
        asm ( "rdtsc\n\t"
              "mov %%edx, %0\n\t"
              "mov %%eax, %1\n\t"
        : "=m"(first), "=m"(second)
        : "m"(a)
        : "eax", "ebx", "ecx", "edx");
        one_half = first;
        two_half = second;
        one_half = one_half << 32;
        one_half ^= two_half;
    }
}
