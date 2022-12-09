    .global strtou256
strtou256:
    push %ebp
    mov %esp, %ebp

    push %esi
    push %edi
    push %ebx

    mov 8(%ebp), %ebx // text
    mov 12(%ebp), %edi
    mov %edi, size_of_text
    mov 16(%ebp), %esi  // buffer

    xor %eax, %eax    // counert for text
    xor %ecx, %ecx
    xor %edx, %edx
    xor %edi, %edi


to_zero:
    cmp $8, %eax
    je preparing_body
    mov %ecx, (%esi, %eax, 4)
    inc %eax
    jmp to_zero
preparing_body:
    xor %eax, %eax
    xor %ecx, %ecx

body:
    cmp size_of_text, %eax
    je exit

    movsbl (%ebx, %eax), %edi
    cmp $'0', %edi
    jl exit
    cmp $'9', %edi
    jg exit
    sub $'0', %edi

    inc %eax
    push %eax
    call u256mul10reg
    pop %eax

    xor %ecx, %ecx
    mov $1, %edx
    add %edi, (%esi, %ecx, 4)
    jnc body
loop:
    cmp $8, %edx
    je body
    add $1, (%esi, %edx, 4)
    jnc body
    inc %edx
    jmp loop
exit:
    pop %ebx
    pop %edi
    pop %esi
    mov %ebp, %esp
    pop %ebp
    ret


    .data
size_of_text:
    .int 0
