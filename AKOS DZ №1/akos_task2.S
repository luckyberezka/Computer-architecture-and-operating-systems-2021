    .global main
main:
    call readi32
body:
    cmp $0, %eax
    jl exit
    call writei32
    dec %eax
    jmp body
exit:
    call finish
