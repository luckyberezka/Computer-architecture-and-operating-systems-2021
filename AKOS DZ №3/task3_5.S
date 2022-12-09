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
    .global MyReadStr
MyReadStr:
    push    %ebp
    mov     %esp, %ebp
    push    %ebx

    mov     $SYS_read, %eax
    mov     $STDIN_FILENO, %ebx
    mov     8(%ebp), %ecx
    mov     12(%ebp), %edx

    int     $0x80

    pop     %ebx
    pop     %ebp
    ret



    .global main
main:
    xor %esi, %esi //result counter
    xor %edi, %edi //local counter
cycle:
    push $BUFFERSIZE
    push $my_memory
    call MyReadStr
    add $8, %esp

    cmp $0, %eax
    je exit


    cmp $0, %edi
    je new_symbol
    dec %edi
    jmp cycle
new_symbol:
    inc %esi
    xor %eax, %eax
    mov my_memory, %al // now byte
loop_for_sdvig:
    shl %al
    jnc preparing_for_repeat
    inc %edi
    jmp loop_for_sdvig
preparing_for_repeat:
    cmp $0, %edi
    je cycle
    dec %edi
    jmp cycle
exit:
    mov %esi, %eax
    call writei32
    call finish



    .data
my_memory:
    .byte ''
