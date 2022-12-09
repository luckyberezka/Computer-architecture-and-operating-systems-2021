    .global process
process:
    push %ebp
    mov %esp, %ebp
    xor %edx, %edx
loop:
    call readi32
    jc print
    sub $4, %esp
    mov %eax, (%esp)
    inc %edx
    jmp loop
print:
    cmp $0, %edx
    jz exit
    mov (%esp), %eax
    call writei32
    add $4, %esp
    sub $1, %edx
    jmp print
exit:
    mov %ebp, %esp
    pop %ebp
    ret

