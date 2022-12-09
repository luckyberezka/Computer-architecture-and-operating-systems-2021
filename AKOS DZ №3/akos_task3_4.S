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
    mov symbols, %eax
    inc %eax
    mov %eax, symbols
body_of_cycle:
    mov my_memory, %al
    cmp $'\n', %al
    je is_n
    cmp $' ', %al
    je is_space
    jmp is_char
preparing_for_exit:
    xor %eax, %eax
    cmp symbols, %eax
    jne checker
    mov %eax, lines
    mov %eax, words
    jmp exit
checker:
    mov my_memory, %al
    cmp $'\n', %al
    je exit
    xor %eax, %eax
    mov lines, %eax
    inc %eax
    mov %eax, lines

exit:
    mov lines, %eax
    call writei32
    mov words, %eax
    call writei32
    mov symbols, %eax
    call writei32

    mov $SYS_exit, %eax
    mov $0, %ebx
    int $0x80






is_space:
    mov status_delimetr, %eax
    mov $1, %eax
    mov %eax, status_delimetr
    jmp cycle



is_n:
    mov lines, %eax
    inc %eax
    mov %eax, lines
    mov status_delimetr, %eax
    mov $1, %eax
    mov %eax, status_delimetr
    jmp cycle



is_char:
    mov status_delimetr, %eax
    cmp $0, %eax
    je end_is_char
    mov status_char, %eax
    cmp $0, %eax
    je end_is_char
    mov words, %eax
    inc %eax
    mov %eax, words
end_is_char:
    mov status_char, %eax
    mov $1, %eax
    mov %eax, status_char

    mov status_delimetr, %eax
    mov $0, %eax
    mov %eax, status_delimetr

    jmp cycle


    .data
my_memory:
    .byte ''
helper:
    .byte '\n'
symbols:
    .int 0
words:
    .int 0
lines:
    .int 0
status_delimetr:
    .int 1
status_char:
    .int 1
