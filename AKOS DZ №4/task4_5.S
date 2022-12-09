    .global sort_by_abs
sort_by_abs:
    push %ebp
    mov %esp, %ebp


    mov 8(%ebp), %ecx // array adress
    mov 12(%ebp), %eax // size
    mov $compare, %edx

    push %edx
    mov $2, %edx
    push %edx
    push %eax
    push %ecx

    call qsort

    add $16, %esp

    mov %ebp, %esp
    pop %ebp
    ret



    .global compare
compare:
    push %ebp
    mov %esp, %ebp

    mov 8(%ebp), %eax
    movsxw (%eax), %eax
    mov 12(%ebp), %ecx
    movsxw (%ecx), %ecx
abs_for_first:
    cmp $0, %eax
    jge abs_for_second
    neg %eax
abs_for_second:
    cmp $0, %ecx
    jge body
    neg %ecx
body:
    xor %edx, %edx
    cmp %ecx, %eax
    je exit
    jl for_less
for_more:
    mov $1, %edx
    jmp exit
for_less:
    mov $-1, %edx
exit:
    mov %edx, %eax
    mov %ebp, %esp
    pop %ebp
    ret



