#include <stdio.h>
#include <pthread.h>

struct Helper {
    pthread_t id;
    long long int now;
};

struct Helper now_thread[10];

void *func(void *arg) {
    struct Helper number = *(struct Helper *)arg;
    if (number.now > 0) {
        pthread_join(now_thread[number.now - 1].id, NULL);
    }
    printf("%lld\n", number.now);
    return 0;
}

int main() {
    for (long long int i = 0; i < 10; ++i) {
        now_thread[i].now = i;
        pthread_create(&now_thread[i].id, NULL, &func, &now_thread[i]);
    }
    pthread_join(now_thread[9].id, NULL);
}
