    .global myhypot
myhypot:
    push %ebp
    mov %esp, %ebp

    movsd 8(%ebp), %xmm0
    movsd 16(%ebp), %xmm1
    mov 24(%ebp), %edx

    mulsd %xmm0, %xmm0
    mulsd %xmm1, %xmm1

    addsd %xmm1, %xmm0

    sqrtsd %xmm0, %xmm2

    movsd %xmm2, (%edx)


    mov %ebp, %esp
    pop %ebp
    ret

