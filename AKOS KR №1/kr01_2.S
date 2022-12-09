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

cycle:
    push $BUFFERSIZE
    mov $my_memory, %edx
    push %edx
    call MyReadStr
    add $8, %esp


    xor %eax, %eax
    mov my_memory, %al
    cmp $'\n', %al
    je next
    cmp $' ', %al
    je cycle



    cmp $'A', %al
    jl continue
    cmp $'Z', %al
    jg continue
    add $32, %al

continue:
    sub $33, %eax
    mov $1, %ecx
    add %ecx, my_array(, %eax, 4)
    jmp cycle
next:
    push $BUFFERSIZE
    mov $my_memory, %edx
    push %edx
    call MyReadStr
    add $8, %esp


    xor %eax, %eax
    mov my_memory, %al
    cmp $'\n', %al
    je print_for_yes
    cmp $' ', %al
    je next


    cmp $'A', %al
    jl prepar
    cmp $'Z', %al
    jg prepar
    add $32, %al

prepar:
    sub $33, %eax
    mov my_array(, %eax, 4), %ecx

    sub $1, %ecx
    cmp $0, %ecx
    jl print_for_no
    mov %ecx, my_array(, %eax, 4)
    jmp next

print_for_yes:
    push $3
    push $my_yes
    call MyWriteStr
    add $8, %esp
    jmp exit
print_for_no:
    push $2
    push $my_no
    call MyWriteStr
    add $8, %esp

exit:
    mov $SYS_exit, %eax
    mov $0, %ebx
    int $0x80

    .data
my_array:
    .skip 400
    .data
my_memory:
    .byte ''
my_yes:
    .ascii "YES"
my_no:
    .ascii "NO"
