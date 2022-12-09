    .global main
main:
    mov $0, %edx
loop:
    call readi32
    jc exit
    add %eax, %edx
    jno loop
    sar $31, %edx
    xor $0x80000000, %edx
exit:
    mov %edx, %eax
    call writei32
    call finish
