    .global aggregate
aggregate:
    push %ebp
    mov %esp, %ebp
    push %edi
    push %esi
    push %ebx

    mov 8(%ebp), %edi  // adress of first elem
    mov 12(%ebp), %esi // adress of stas
    xor %ebx, %ebx

    cmp %ebx, %edi
    je take_mid

    mov (%edi), %eax
    mov %eax, (%esi)  // min

    mov (%edi), %eax
    mov %eax, 4(%esi) // max

    mov $0, %eax
    mov %eax, 8(%esi) // midl


    xor %eax, %eax // %edx:%eax == sum
    xor %edx, %edx // sum
    xor %ebx, %ebx // counter of elems
body:
    cmp $0, %edi
    je take_mid

    mov (%edi), %ecx
    push %edi
    push %ebx


check_min_part:
    cmp (%esi), %ecx
    jl take_new_min
check_max_part:
    cmp 4(%esi), %ecx
    jg take_new_max
check_midl_part:

    mov %ecx, %ebx
    sar $31, %ebx


    add %ecx, %eax
    adc %ebx, %edx


to_body:


    pop %ebx
    pop %edi
    inc %ebx
    mov 4(%edi), %edi
    jmp body
take_mid:
    cmp $0, %ebx
    jne preparing_for_answer
    mov %ebx, (%esi)
    mov %ebx, 4(%esi)
    mov %ebx, 8(%esi)
    jmp exit
preparing_for_answer:
    idiv %ebx
    mov %eax, 8(%esi)
exit:
    pop %ebx
    pop %esi
    pop %edi
    mov %ebp, %esp
    pop %ebp
    ret


take_new_min:
    mov %ecx, (%esi)
    jmp check_max_part
take_new_max:
    mov %ecx, 4(%esi)
    
    
    .global main
main:
    mov $a, %eax
    add $4, %eax
    mov $b, %edx
    mov %edx, (%eax)

    mov $b, %eax
    add $4, %eax
    mov $c, %edx
    mov %edx, (%eax)

    mov $c, %eax
    add $4, %eax
    mov $d, %edx
    mov %edx, (%eax)

    push $stas
    push $c
    call aggregate

    mov $stas, %ecx

    mov (%ecx), %eax
    call writei32

    mov 4(%ecx), %eax
    call writei32

    mov 8(%ecx), %eax
    call writei32

    call finish


    .data
stas:
    .int 0
    .int 0
    .int 0


a:
    .int 1
    .int 0
b:
    .int 8
    .int 0
c:
    .int -4
    .int 0
d:
    .int 8
    .int 0
    jmp check_midl_part

