#include <sys/syscall.h>

void _start() {
    char *mystr = "hello world\n";
    // обратиться к системному вызову write
    asm volatile ( "mov $4, %%eax\n\t"
                   "mov $1, %%ebx\n\t"
                   "mov %0, %%ecx\n\t"
                   "mov $12, %%edx\n\t"
                   "int $0x80"
    : 
    : "m"(mystr)
    : "eax", "ebx", "ecx", "edx" );

    asm ( "mov $1, %%eax\n\t"
          "mov $0, %%ebx\n\t"
          "int $0x80"
    : 
    : 
    : "eax", "ebx", "ecx", "edx" );
}
