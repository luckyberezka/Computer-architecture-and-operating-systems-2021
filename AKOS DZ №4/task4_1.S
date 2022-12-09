 #include <sys/syscall.h>


    .global alloc_mem
alloc_mem:
    push %ebp
    mov %esp, %ebp
    push %edi
    push %esi
    push %ebx
body:
    mov 8(%ebp), %edi // new block size
    mov $0, %ecx // prev block adress
    mov freelist, %ebx // now block adress
cycle:
    cmp $0, %ebx
    je no_place_in_list

    cmp (%ebx), %edi
    jg to_cycle

    mov $1, %eax
    mov (%ebx, %eax, 4), %edx

    cmp $0, %ecx
    je del_if_first

del_if_not_first:
    mov %edx, 4(%ecx) // ecx + 4?
    jmp to_exit

del_if_first:
    mov %edx, freelist

to_exit:
    add $4, %ebx
    mov %ebx, %eax

    jmp exit

to_cycle:
    mov %ebx, %ecx
    mov $1, %eax
    mov (%ebx, %eax, 4), %ebx
    jmp cycle




no_place_in_list:
    xor %eax, %eax
    xor %ecx, %ecx // counter for align
    mov $4, %ebx
loop_for_align:
    xor %edx, %edx
    mov %edi, %eax

    add %ecx, %eax
    jnc continue_loop

    xor %eax, %eax
    jmp exit
continue_loop:
    mov $4, %ebx
    div %ebx
    cmp $0, %edx
    je do_actual_size
    inc %ecx
    jmp loop_for_align

do_actual_size:
    add %ecx, %edi
    jnc metka
    xor %eax, %eax
    jmp exit
metka:
    cmp $0, %edi
    jne preparing_for_brk
    mov $4, %edi
preparing_for_brk:
    add $4, %edi
    jnc do_my_brk
    xor %eax, %eax
    jmp exit
do_my_brk:

    mov $SYS_brk, %eax
    mov $0, %ebx
    int $0x80

    mov %eax, heapstart

    mov %eax, %ebx
    add %edi, %ebx
    jnc after_problem_place
    xor %eax, %eax
    jmp exit
after_problem_place:
    mov $SYS_brk, %eax
    int $0x80

    cmp %eax, %ebx
    je successful
    xor %eax, %eax
    jmp exit
successful:
    mov heapstart, %eax

    sub $4, %edi

    mov %edi, (%eax)
    add $4, %eax
exit:
    pop %ebx
    pop %esi
    pop %edi
    mov %ebp, %esp
    pop %ebp
    ret

    .data
heapstart:
    .int 0

