    .global mul2
mul2:
    push %ebp
    mov %esp, %ebp
    push %edi

    mov 8(%ebp), %edi
    mov (%edi), %eax
    mov $2, %ecx

    imul %ecx
    jo exit

    mov %eax, (%edi)

exit:
    pop %edi
    mov %ebp, %esp
    pop %ebp
    ret
