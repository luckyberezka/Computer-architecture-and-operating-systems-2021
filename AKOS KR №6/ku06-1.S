    .global run_on_stack
run_on_stack:
    push %ebp
    mov %esp, %ebp


    push %edi
    push %esi
    push %ebx

    mov 8(%ebp), %eax // func
    mov 12(%ebp), %esi // arg
    mov 16(%ebp), %ebx // new stack

    mov %esp, %edi
    
    mov %ebx, %esp
    
    push %esi
    call %eax
    
    mov %edi, %esp
    
    
    pop %ebx
    pop %esi
    pop %edi
    mov %ebp, %esp
    pop %ebp
    ret
