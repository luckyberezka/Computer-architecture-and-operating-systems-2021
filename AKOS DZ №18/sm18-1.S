    .global spin_lock
spin_lock:
    push %ebp
    mov %esp, %ebp
loop:
    mov $0, %eax
    mov $1, %ecx
    mov 8(%ebp), %edx
    lock cmpxchg %ecx, (%edx)
    jz ender
    pause
    jmp loop
ender:
    mov %ebp, %esp
    pop %ebp
    ret




    .global spin_unlock
spin_unlock:
    push %ebp
    mov %esp, %ebp

    mov 8(%ebp), %eax
    mov $0, %ecx
    mov %ecx, (%eax)

    mov %ebp, %esp
    pop %ebp
    ret
