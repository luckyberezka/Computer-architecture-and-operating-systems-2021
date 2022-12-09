    .global main
main:
    mov $0, %ecx
    mov $0, %edx
body:
    call readi32
    jc exit
    test $1, %eax
    jz even
    jnz odd
even:
    add %eax, %ecx
    jmp body
odd:
    add %eax, %edx
    jmp body
exit:
    mov %ecx, %eax
    call writei32
    mov %edx, %eax
    call writei32
    call finish
