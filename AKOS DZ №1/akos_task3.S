    .global main
main:
    call readi64
    shl %eax
    rcl %edx
    mov %edx, %ecx
    mov %eax, %ebx
    call readi64
    shr %edx
    rcr %eax
    sub %eax, %ebx
    sbb %edx, %ecx
    mov %ebx, %eax
    mov %ecx, %edx
    call writei64
    call finish
