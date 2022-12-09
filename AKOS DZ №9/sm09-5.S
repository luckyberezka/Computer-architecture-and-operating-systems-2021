    .global dot_product
dot_product:
    push %ebp
    mov %esp, %ebp
    push %ebx
    push %edi
    push %esi

    mov 8(%ebp), %eax // n

    mov 12(%ebp), %ebx // adress of first
    mov 16(%ebp), %ecx // adress of second
    mov 20(%ebp), %edx // adress of result

    xor %esi, %esi    // global conuter

    xorpd %xmm0, %xmm0 // result
    xorpd %xmm1, %xmm1 // pack for first
    xorpd %xmm2, %xmm2 // pack for second

    cmp $0, %eax
    je exit

loop:
    mov %eax, %edi
    sub %esi, %edi
    cmp $4, %edi
    jge step_for_four
step_for_one:
    movss (%ebx, %esi, 4), %xmm1
    movss (%ecx, %esi, 4), %xmm2
    add $1, %esi
    jmp pack_is_full

step_for_four:
    movups (%ebx, %esi, 4), %xmm1
    movups (%ecx, %esi, 4), %xmm2
    add $4, %esi
    jmp pack_is_full

exit:
    movss %xmm0, (%edx)
    pop %esi
    pop %edi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret


pack_is_full:
    mulps %xmm2, %xmm1
    xorpd %xmm2, %xmm2
    haddps %xmm2, %xmm1
    haddps %xmm2, %xmm1
    addss %xmm1, %xmm0
    cmp %esi, %eax
    je exit
    xorpd %xmm1, %xmm1
    xorpd %xmm2, %xmm2
    jmp loop
