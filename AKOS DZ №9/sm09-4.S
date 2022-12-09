    .global myexp
myexp:
    push %ebp
    mov %esp, %ebp
    push %edi

    movsd 8(%ebp), %xmm0 // start x
    mov 16(%ebp), %edi // adress for result

    mov $1, %eax

    cvtsi2ss %eax, %xmm1 // now x

    cvtss2sd %xmm1, %xmm2 // one for sum

    cvtss2sd %xmm1, %xmm3 // now factor

    xorpd %xmm4, %xmm4 // counter

    cvtss2sd %xmm1, %xmm5 // prev res

    cvtss2sd %xmm1, %xmm6 // now res

    cvtss2sd %xmm1, %xmm1

loop:
    mulsd %xmm0, %xmm1 // take new x
    addsd %xmm2, %xmm4 // take new count
    mulsd %xmm4, %xmm3 // new factor

    movsd %xmm1, %xmm7 // now adder elem

    divsd %xmm3, %xmm7

    addsd %xmm7, %xmm6

    comisd %xmm6, %xmm5
    je end_loop

    movsd %xmm6, %xmm5
    jmp loop

end_loop:

    movsd %xmm6, (%edi)

    pop %edi
    mov %ebp, %esp
    pop %ebp
    ret
