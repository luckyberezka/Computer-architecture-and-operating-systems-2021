    .global main
main:
    xor %eax, %eax
    xor %ecx, %ecx
    xor %edx, %edx
loop:
    call readi32
    jc exit
    cmp $0, %eax
    jl mypop
    mov %eax, %ecx
    call readi32 // ecx == number version, eax == value
    jmp mypush
exit:
    call finish



mypush:
    mov size, %edx
    add $1, %edx
    mov %edx, size

    mov stack_second(, %ecx, 4), %edx
    mov size, %ecx
    mov %edx, stack_first(, %ecx, 8)
    mov %eax, stack_first+4(, %ecx, 8)
    mov %ecx, stack_second(, %ecx, 4)
    jmp loop



mypop:
    neg %eax
    mov %eax, %ecx // ecx == index
    mov stack_second(, %ecx, 4), %edx // %edx == now
    cmp $0, %edx
    jz is_zero
not_zero:
    mov stack_first+4(, %edx, 8), %eax
    call writei32
    mov stack_first(, %edx, 8), %eax
    mov %eax, stack_second(, %ecx, 4)
    jmp loop
is_zero:
    mov $1, %eax
    neg %eax
    call writei32
    jmp loop



    .data
stack_first:
    .int 0
    .int 0
    .skip 8000
stack_second:
    .int 0
    .skip 4000
size:
    .int 0
