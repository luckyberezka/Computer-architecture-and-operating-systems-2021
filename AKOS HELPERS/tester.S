 #include <sys/syscall.h>


// источник функций MyWriteStr, MyReadStr https://github.com/lll-phill-lll/hse_caos_207_seminars/blob/master/3sem-asm/add.S

    .equ STDIN_FILENO, 0
    .equ STDOUT_FILENO, 1
    .equ BUFFERSIZE, 1
MyWriteStr:
    push    %ebp
    mov     %esp, %ebp
    push    %ebx

    mov     $SYS_write, %eax
    mov     $STDOUT_FILENO, %ebx
    mov     8(%ebp), %ecx
    mov     12(%ebp), %edx

    int     $0x80

    pop     %ebx
    pop     %ebp
    ret


    .global main
main:
    call readi32
    add $'0', %eax
    mov %eax, (%esp)
    mov     $SYS_write, %eax
    mov     $STDOUT_FILENO, %ebx
    mov     %esp, %ecx
    mov     $1, %edx

    int     $0x80

    call finish

    .data
pidor:
    .byte '0'

