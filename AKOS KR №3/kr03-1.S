    .global discrim
discrim:
    push %ebp
    mov %esp, %ebp

    movss 8(%ebp), %xmm0 // a
    movss 12(%ebp), %xmm1 // b
    movss 16(%ebp), %xmm2 //c
    mov 20(%ebp), %eax // adress for result

    mulss %xmm1, %xmm1

    mov $4, %ecx
    cvtsi2ss %ecx, %xmm3 // 4

    mulss %xmm0, %xmm3
    mulss %xmm2, %xmm3

    subss %xmm3, %xmm1

    movss %xmm1, (%eax)

    mov %ebp, %esp
    pop %ebp
    ret
