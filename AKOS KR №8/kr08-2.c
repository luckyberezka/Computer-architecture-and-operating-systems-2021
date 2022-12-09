#include <unistd.h>
#include <stdio.h>
#include <wait.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdatomic.h>
#include <stdlib.h>

struct Helper {
    pthread_t id;
    long long int now;
};


void fun0(int no);
void fun1(void);
int get_result_state(void);

struct Item * _Atomic tail_list = NULL;

void *func(void *arg) {
    int a, b;
    if (scanf("%d", &a) < 0) {
        return NULL;
    }
    if (a == 0) {
        if (scanf("%d", &b) < 0) {
            return NULL;
        }
    }
    if (a == 1) {
        fun1();
    } else {
        fun0(b);
    }
    return NULL;
}


int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    int k;
    if (scanf("%d", &k) < 0) {
        return 1;
    }
    if (k < n) {
        n = k;
    }
    while (k > 0) {
        struct Helper thr[n];
        for (int i = 0; i < n; ++i) {
            thr[i].now = k;
            pthread_create(&thr[i].id, NULL, func, &thr[i]);
        }
        for (int i = 0; i < n; ++i) {
            pthread_join(thr[i].id, NULL);
        }
        k -= n;
        if (k < n) {
            n = k;
        }
    }
    printf("%d\n", get_result_state());
}
