#include <setjmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *func(void *arg) {
    int nmbr;
    if (scanf("%d", &nmbr) != EOF) {
        pthread_t id;
        pthread_create(&id, NULL, &func, &nmbr);
        pthread_join(id, NULL);
        printf("%d\n", nmbr);
    }
    return 0;
}

int main() {
    int nmbr = 0;
    pthread_t id;
    pthread_create(&id, NULL, &func, &nmbr);
    pthread_join(id, NULL);
}
