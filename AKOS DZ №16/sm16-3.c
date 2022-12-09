#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <setjmp.h>

jmp_buf ebuf[10];
int exep[10];
int top = 0;

jmp_buf* try(int exception) {
    exep[top] = exception;
    ++top;
    return &ebuf[top - 1];
}
void endtry() {
    --top;
}
void throw(int exception) {
    while (1) {
        endtry();
        if (top < 0) {
            _exit(13);
        }
        if (exep[top] == exception) {
            longjmp(ebuf[top], 1);
        }
    }
}
