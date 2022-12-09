#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <malloc.h>
#include <stdatomic.h>
#include <limits.h>
#include <unistd.h>
#include <math.h>

struct Helper {
    pthread_t id;
    unsigned long long now;
};

pthread_cond_t cond;
pthread_mutex_t mutex;
unsigned long long for_prnt = 0;

void *func(void *arg) {
    struct Helper number = *(struct Helper *)arg;


    while (1) {

        int flag = 1;
        if (number.now == 1) {
            ++number.now;
            continue;
        }
        unsigned long long orel = (unsigned long long)sqrt(number.now);
        for (unsigned long long i = 2; i <= orel + 1; ++i) {
            if (number.now % i == 0) {
                flag = 0;
                break;
            }
        }

        pthread_mutex_lock(&mutex);
        if (flag) {
            for_prnt = number.now;
            pthread_cond_signal(&cond);
        }
        pthread_mutex_unlock(&mutex);

        ++number.now;
    }

    return 0;
}

int main() {
    unsigned long long base;
    int count;
    if (scanf("%llu", &base) < 0) {
        return 0;
    }
    if (scanf("%d", &count) < 0) {
        return 0;
    }

    struct Helper thr;
    thr.now = base;
    pthread_create(&thr.id, NULL, func, &thr);

    while (count > 0) {
        unsigned long long pirat = 0;

        pthread_mutex_lock(&mutex);

        pthread_cond_wait(&cond, &mutex);
        pirat = for_prnt;

        pthread_mutex_unlock(&mutex);

        --count;
        printf("%llu\n", pirat);
    }
}
