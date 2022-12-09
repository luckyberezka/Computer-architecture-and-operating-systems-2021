    .global free_mem
free_mem:
    push %ebp
    mov %esp, %ebp

    mov 8(%ebp), %ecx

    sub $4, %ecx
    mov freelist, %edx
    mov %edx, 4(%ecx)
    mov %ecx, freelist


    mov %ebp, %esp
    pop %ebp
    ret


    .global freelist
    .data
freelist:
    .int 0
