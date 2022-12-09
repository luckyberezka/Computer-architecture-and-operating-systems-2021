 #include <sys/syscall.h>


// источник функций MyWriteStr, MyReadStr https://github.com/lll-phill-lll/hse_caos_207_seminars/blob/master/3sem-asm/add.S

MyWriteStr:
    push    %ebp
    mov     %esp, %ebp
    push    %ebx

    mov     $SYS_write, %eax
    mov     $1, %ebx
    mov     8(%ebp), %ecx
    mov     12(%ebp), %edx

    int     $0x80

    pop     %ebx
    pop     %ebp
    ret




 .global writeu32
writeu32:
    push %ebp
    mov %esp, %ebp

    push %edi
    push %esi // last val

    xor %edi, %edi
    xor %eax, %eax
    xor %edx, %edx
    cmp $0, %ecx
    jz dlc_exit

body:
    cmp $0x0, %ecx
    jz printer

    mov %ecx, %esi

    mov $-858993459, %edx
    mov %edx, %eax
    mul %ecx
    mov %edx, %eax
    shr $3, %eax


    mov %eax, %ecx

    mov $10, %edx
    mul %edx
    mov %esi, %edx
    sub %eax, %edx // сейчас в edx остаток текущий
    // далее в цикле сэвти остаются ecx && edx






    inc %edi
    add $'0', %dl

    xor %eax, %eax
    sub $1, %esp
    mov %al, (%esp)
    mov %dl, (%esp)


    jmp body
printer:
    xor %eax, %eax
    xor %edx, %edx

loop_of_printer:
    mov %esp, %eax
    push %edi
    push %eax
    call MyWriteStr
    add $8, %esp
exit:
    add %edi, %esp

    pop %esi
    pop %edi
    mov %ebp, %esp
    pop %ebp
    ret

dlc_exit:
    mov $'0', %eax
    push %eax
    mov %esp, %eax

    push $1
    push %eax
    call MyWriteStr
    add $12, %esp

    pop %esi
    pop %edi
    mov %ebp, %esp
    pop %ebp
    ret



//    .global main
//main:
//    call readi32
//    mov %eax, %ecx
//    mov $0xffffffff, %ecx
//    call writeu32

//    call readi32
//    mov %eax, %ecx
//    call writeu32


//    call readi32
//    mov %eax, %ecx
//    call writeu32


//    call finish

