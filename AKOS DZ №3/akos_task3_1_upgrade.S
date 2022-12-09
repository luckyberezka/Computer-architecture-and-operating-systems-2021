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
    xor %edi, %edi // couner

cycle:
    xor %eax, %eax
    push $BUFFERSIZE
    mov $my_memory, %edx
    push %edx
    call MyReadStr
    add $8, %esp
    cmp $0, %eax
    je preparing_for_exit
body_of_cycle:
    mov my_memory, %al
    cmp $'\n', %al
    jne next_check
    xor %edi, %edi
next_check:
    cmp $'\n', %al
    je print_now_value
    inc %edi
    cmp $11, %edi
    jnz print_now_value
print_dlc_n:
    mov $1, %edi

    push $BUFFERSIZE
    mov $helper, %edx
    push %edx
    call MyWriteStr
    add $8, %esp


print_now_value:
    push $BUFFERSIZE
    push $my_memory
    call MyWriteStr
    add $8, %esp
    jmp cycle
preparing_for_exit:
    mov my_memory, %al
    cmp $1, %al
    je exit
    cmp $'\n', %al
    je exit

    push $BUFFERSIZE
    mov $helper, %edx
    push %edx
    call MyWriteStr
    add $8, %esp


exit:
    mov $SYS_exit, %eax
    mov $0, %ebx
    int $0x80



    .data
my_memory:
    .byte ''
helper:
    .byte '\n'
