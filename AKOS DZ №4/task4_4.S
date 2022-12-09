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
    mov %ebp, %esp
    pop     %ebp
    ret


    .global print_tb
print_tb:
    push %ebp
    mov %esp, %ebp
    push %edi
    push %esi
    mov %ebp, %esi
    mov $print_tb, %edi
cycle:
    // print now %edi

    call printer

    cmp $main, %edi
    je super_exit

    mov $9000, %edx

    xor %edi, %edi
    mov 4(%esi), %eax
for_main:
    cmp $main, %eax
    jl for_foo

    mov %eax, %ecx
    sub $main, %ecx
    cmp %ecx, %edx

    jl for_foo
    mov %ecx, %edx
    mov $main, %edi
for_foo:
    cmp $foo, %eax
    jl for_bar

    mov %eax, %ecx
    sub $foo, %ecx
    cmp %ecx, %edx

    jl for_bar
    mov %ecx, %edx
    mov $foo, %edi

for_bar:
    cmp $bar, %eax
    jl for_print

    mov %eax, %ecx
    sub $bar, %ecx
    cmp %ecx, %edx


    jl for_print
    mov %ecx, %edx
    mov $bar, %edi
for_print:
    cmp $print_tb, %edi
    jl to_cycle

    mov %eax, %ecx
    sub $print_tb, %ecx
    cmp %ecx, %edx


    jl to_cycle
    mov %ecx, %edx
    mov $print_tb, %edi
to_cycle:
    mov (%esi), %esi
    jmp cycle

super_exit:
    pop %esi
    pop %edi
    mov %ebp, %esp
    pop %ebp
    ret

printer:
    push %ebp
    mov %esp, %ebp
print_for_main:
    cmp $main, %edi
    jne print_for_foo

    push $str_for_foo-str_for_main
    push $str_for_main
    call MyWriteStr
    add $8, %esp
    jmp exit
print_for_foo:
    cmp $foo, %edi
    jne print_for_bar

    push $str_for_bar-str_for_foo
    push $str_for_foo
    call MyWriteStr
    add $8, %esp
    jmp exit

print_for_bar:
    cmp $bar, %edi
    jne print_for_print

    push $str_for_print-str_for_bar
    push $str_for_bar
    call MyWriteStr
    add $8, %esp
    jmp exit
print_for_print:
    push $ustal-str_for_print
    push $str_for_print
    call MyWriteStr
    add $8, %esp
exit:
    mov %ebp, %esp
    pop %ebp
    ret


    .data
str_for_main:
    .ascii "main\n"
str_for_foo:
    .ascii "foo\n"
str_for_bar:
    .ascii "bar\n"
str_for_print:
    .ascii "print_tb\n"
ustal:
    .ascii "end"
