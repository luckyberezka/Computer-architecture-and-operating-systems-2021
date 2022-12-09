#include <stdio.h>
#include <stdlib.h>

struct data {
    int (*get_rank)(const struct data *);

    char secret[];
};

int compare(const void *x1, const void *x2) {
    if ((*(struct data **)x1)->get_rank(*(struct data **)x1) < (*(struct data **)x2)->get_rank(*(struct data **)x2)) {
        return -1;
    }
    if ((*(struct data **)x1)->get_rank(*(struct data **)x1) > (*(struct data **)x2)->get_rank(*(struct data **)x2)) {
        return 1;
    }
    return 0;
}

void rank_sort(struct data *elements[], size_t size) {
    qsort(elements, size, sizeof(struct data *), compare);
}

