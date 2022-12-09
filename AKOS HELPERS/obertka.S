.global kek
 .text
kek:
 push %ebp
 mov %esp, %ebp
 push %ebx
 push %esi
 push %edi

 mov 8(%ebp), %ebx // adress
 mov 12(%ebp), %esi // size
 
 
 
 push %esi
 push %ebx


 call sort_by_abs
 add $8, %esp
 
 xor %ecx, %ecx
 cycle:
     cmp %esi, %ecx
     je exit
     xor %eax, %eax
     movsxw (%ebx, %ecx, 2), %eax
     call writei32
     inc %ecx
     jmp cycle
exit:
 pop %edi
 pop %esi
 pop %ebx
 mov %ebp, %esp
 pop %ebp
 ret
