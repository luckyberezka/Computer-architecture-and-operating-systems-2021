    .global partition
partition:
    push %ebp
    mov %esp, %ebp
max_part:
    xor %eax, %eax
    xor %ecx, %ecx
    xor %edx, %edx
loop_max_part:
    cmp %esi, %edx
    jz equal_part
    movsxw (%ebx, %edx, 2), %ecx
    inc %edx
    cmp %edi, %ecx
    jle loop_max_part
    inc %eax
    mov %eax, max_size
    push %ecx
    jmp loop_max_part



equal_part:
    xor %eax, %eax
    xor %ecx, %ecx
    xor %edx, %edx
loop_equal_part:
    cmp %esi, %edx
    jz min_part
    movsxw (%ebx, %edx, 2), %ecx
    inc %edx
    cmp %edi, %ecx
    jne loop_equal_part
    push %ecx
    jmp loop_equal_part



min_part:
    xor %eax, %eax
    xor %ecx, %ecx // cx == now value
    xor %edx, %edx // counter
loop_min_part:
    cmp %esi, %edx
    jz printer
    movsxw (%ebx, %edx, 2), %ecx
    inc %edx
    cmp %edi, %ecx
    jge loop_min_part
    inc %eax
    mov %eax, min_size
    push %ecx
    jmp loop_min_part


printer:
    xor %eax, %eax
    xor %ecx, %ecx
    xor %edx, %edx
loop_printer:
    cmp %esi, %edx
    jz exit
    pop %ecx
    mov %ecx, (%ebx, %edx, 2)
    inc %edx
    jmp loop_printer
exit:
    xor %edx, %edx
    mov min_size, %edx
    cmp $0, %edx
    jnz end_exit
    mov $1, %edx
end_exit:
    mov %ebp, %esp
    pop %ebp
    ret   // warning!


    .data
min_size:
    .int 0
max_size:
    .int 0




