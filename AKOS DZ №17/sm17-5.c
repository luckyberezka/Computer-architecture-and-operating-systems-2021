#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <malloc.h>
#include <stdatomic.h>
#include <limits.h>
#include <unistd.h>


struct Item {
    struct Item *next;
    long long value;
};

struct Helper {
    pthread_t id;
    long long int now;
};

struct Item * _Atomic tail_list = NULL;

void *func(void *arg) {
    struct Helper number = *(struct Helper *)arg;
    for (int i = 0; i < 1000; ++i) {
        struct Item *tmp = malloc(sizeof(*tmp));
        tmp->next = tail_list;
        tmp->value = i + 1000 * number.now;
        while (!atomic_compare_exchange_weak(&tail_list, &tmp->next, tmp));
        sched_yield();
    }

    return 0;
}

int main() {
    pthread_attr_t tattr;
    pthread_attr_setstacksize(&tattr, sysconf(_SC_THREAD_STACK_MIN));




    struct Helper thr[100];
    for (int i = 0; i < 100; ++i) {
        thr[i].now = i;
        pthread_create(&thr[i].id, &tattr, func, &thr[i]);
    }
    for (int i = 0; i < 100; ++i) {
        pthread_join(thr[i].id, NULL);
    }
    while (tail_list != NULL) {
        printf("%lld\n", tail_list->value);
        struct Item *tmp = tail_list;
        tail_list = tail_list->next;
        free(tmp);
    }
}
