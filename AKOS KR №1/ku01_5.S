    .global delegate
delegate:
    push %ebp
    mov %esp, %ebp
    push %edi
    push %ebx

    xor %edx, %edx
    mov $4, %ebx
    mov 8(%ebp), %eax
    mul %ebx


    mov 12(%ebp), %ecx

    mov %ebp, %edi
    add $12, %edi

    add %edi, %eax
loop:
    cmp %edi, %eax
    je loop_ender

    mov (%eax), %ebx
    push %ebx

    sub $4, %eax
    jmp loop

loop_ender:
    call *%ecx

    mov %ebp, %esp
    sub $8, %esp
    pop %ebx
    pop %edi
    mov %ebp, %esp
    pop %ebp
    ret



    .global main
main:
    push $400
    push $200
    push $2
    push $helper
    push $3

    call delegate


    call finish


helper:
    push %ebp
    mov %esp, %ebp

    mov 8(%ebp), %eax
    call writei32

    mov 12(%ebp), %eax
    call writei32

    mov 16(%ebp), %eax
    call writei32


    mov %ebp, %esp
    pop %ebp
    ret

