 #include <sys/syscall.h>

    .global main
main:
    sub $4, %esp
    push %ebp
    mov %esp, %ebp
    add $8, %esp


    call process
    call writei32

    mov $SYS_exit, %eax
    mov $0, %ebx
    int $0x80


