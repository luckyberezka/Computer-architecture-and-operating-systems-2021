    .global main
main:
    mov $-1, %eax

    mov %eax, %edx
    sar $31, %edx

    call writei64
    call finish
