    .global list_reverse
list_reverse:
    push %ebp
    mov %esp, %ebp
    xor %eax, %eax
    xor %ecx, %ecx
    xor %edx, %edx
    mov head, %eax
loop:
    cmp $0, %eax      // eax = now
    jz exit            // edx = prev            edx = eax     eax = ecx; ecx = 8(%eax);
    mov 8(%eax) , %ecx  // ecx =  next
    mov %edx, 8(%eax)
    mov %eax, %edx
    mov %ecx, %eax
    jmp loop
exit:
    mov %edx, head
    mov %ebp, %esp
    pop %ebp
    ret
