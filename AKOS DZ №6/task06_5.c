#include <stdio.h>
#include <stdlib.h>

struct RandomOperations;

typedef struct RandomGenerator {
    struct RandomOperations *ops;
    unsigned int value, a, c, m;
} RandomGenerator;

int nextfunc(RandomGenerator *rr) {
    rr->value = ((rr->a * rr->value) % rr->m + rr->c) % rr->m;
    return rr->value;
}

void destroyfunc(RandomGenerator *rr) {
    if (rr) {
        free(rr);
    }
}

typedef int (*next_t)(RandomGenerator *);

typedef void (*destroy_t)(RandomGenerator *);

struct RandomOperations {
    next_t next;
    destroy_t destroy;
};

struct RandomOperations helper = {nextfunc, destroyfunc};

RandomGenerator *random_create(int seed) {
    RandomGenerator *tmp = malloc(sizeof(*tmp));

    tmp->ops = &helper;
    tmp->value = seed;
    tmp->a = 1103515245;
    tmp->c = 12345;
    tmp->m = 2147483648;
    return tmp;
}

