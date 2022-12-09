    .global обработка
обработка:
    xor %eax, %eax
    push %eax
    push %ebp
    mov %esp, %ebp
    mov 8(%ebp), %edx
    mov %edx, 4(%ebp)
    mov %eax, 8(%ebp)
    push %ebx
    push %esi
    push %edi

    add $1, %esi
    add $13, %edi

body:
    xor %eax, %eax // now sum
    cmp $0, %ebx
    je exit

    add (%ebx, %esi, 4), %eax
    add (%ebx, %edi, 4), %eax

    mov %eax, %edx
    shr $31, %edx
    add %edx, %eax
    sar %eax

    add %eax, 8(%ebp)
    mov (%ebx), %ebx


    jmp body
exit:
    pop %edi
    pop %esi
    pop %ebx
    mov %ebp, %esp
    pop %ebp
    ret

