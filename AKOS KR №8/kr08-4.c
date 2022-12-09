#include <memory.h>
#include <pthread.h>
#include <malloc.h>

enum {
    NumberOfThreads = 4
};

struct Helper {
    pthread_t id;
    void *local_res;
    const void* begin;
    size_t sz;
    size_t n;
    size_t step;
    void (*binary_op)(void* state, void* el);
    size_t fr;
};

int MyMin(int a, int b) {
    if (a <= b) {
        return a;
    }
    return b;
}

void *func(void *arg) {
    struct Helper state = *(struct Helper *)arg;
    for (size_t i = state.fr; i < MyMin(state.fr + state.step, state.n); ++i) {
        state.binary_op(state.local_res, (void *)state.begin + i * state.sz);
    }
    return 0;
}

void std_accumulate(void* result, const void* begin, size_t size, size_t n,
                    void* init_state, void (*binary_op)(void* state, void* el)) {
    struct Helper thr[NumberOfThreads];
    size_t from = 0;
    size_t ost = n % NumberOfThreads;

    for (int i = 0; i < NumberOfThreads; ++i) {
        thr[i].begin = begin;
        thr[i].binary_op = binary_op;
        thr[i].n = n;
        thr[i].sz = size;
        thr[i].step = n / NumberOfThreads;
        
        if (ost) {
            thr[i].step++;
            ost--;
        }
        
        thr[i].fr = from;
        from += thr[i].step;


        thr[i].local_res = malloc(size);
        memcpy(thr[i].local_res, init_state, size);
        pthread_create(&thr[i].id, NULL, func, &thr[i]);
    }
    memcpy(result, init_state, size);
    for (int i = 0; i < NumberOfThreads; ++i) {
        pthread_join(thr[i].id, NULL);
        binary_op(result, thr[i].local_res);
        free(thr[i].local_res);
    }
}
