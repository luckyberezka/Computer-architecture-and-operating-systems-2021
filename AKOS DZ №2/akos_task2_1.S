    .global A, B, R, process

.global main
process:
    push %ebp
    mov %esp, %ebp
body:
    xor %eax, %eax
    mov A, %ecx
    mov B, %edx
loop:
    shr $1, %ecx
    jnc next
    add %edx, %eax
next:
    shl $1, %edx
    cmp $0, %edx
    jnz loop
exit:
    mov %eax, R
    mov %ebp, %esp
    pop %ebp
    ret


    .data
A:
    .int 0
B:
    .int 0
R:
    .int 0
