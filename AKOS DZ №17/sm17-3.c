#include <stdio.h>
#include <pthread.h>

enum {
    ArrSize = 3,
    CountIter = 1000000
};

struct Helper {
    pthread_t id;
    long long int now;
};

double arr[ArrSize] = {0, 0, 0};
pthread_mutex_t rr_lock;

void *func(void *arg) {
    struct Helper number = *(struct Helper *)arg;
    for (int i = 0; i < CountIter; ++i) {
        pthread_mutex_lock(&rr_lock);
        arr[number.now] += 100.0 + (double) number.now * 100.0;
        arr[(number.now + 1) % 3] -= 101.0 + (double) number.now * 100.0;
        pthread_mutex_unlock(&rr_lock);
    }
    return 0;
}

int main() {
    struct Helper thr[ArrSize];
    for (int i = 0; i < ArrSize; ++i) {
        thr[i].now = i;
        pthread_create(&thr[i].id, NULL, func, &thr[i]);
    }
    for (int i = 0; i < ArrSize; ++i) {
        pthread_join(thr[i].id, NULL);
    }
    for (int i = 0; i < ArrSize; ++i) {
        printf("%.10g\n", arr[i]);
    }
}
