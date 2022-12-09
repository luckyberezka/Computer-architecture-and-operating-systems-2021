#include <stdio.h>
#include <stdlib.h>

struct MyArray {
    size_t sz_;
    size_t capacity_;
    char *pointer_;
} array = {};

void Append(struct MyArray *arr, char elem, int *flag) {
    if (arr->sz_ + 1 > arr->capacity_) {
        //__builtin_mul_overflow();
        size_t updatecap = 2 * (arr->capacity_ + 1);
        char *tmp = realloc(arr->pointer_, updatecap * sizeof(*tmp));
        if (!tmp) {
            *flag = 1;
            return;
        }
        arr->pointer_ = tmp;
        arr->capacity_ = updatecap;
    }
    arr->pointer_[arr->sz_++] = elem;
}

char *getline2(FILE *f) {
    if (!f) {
        return NULL;
    }
    char *result = realloc(NULL, sizeof(*result));
    if (result == NULL) {
        return result;
    }

    struct MyArray helper;
    helper.capacity_ = 1;
    helper.sz_ = 0;
    helper.pointer_ = result;

    int flag = 0;
    char now = '\0';

    while (fscanf(f, "%c", &now) != EOF) {
        Append(&helper, now, &flag);
        if (flag == 1) {
            free(helper.pointer_);
            return NULL;
        }
        if (now == '\n') {
            break;
        }
    }
    if (helper.sz_ == 0) {
        free(helper.pointer_);
        return NULL;
    }
    char ender = '\0';
    Append(&helper, ender, &flag);
    if (flag == 1) {
        free(helper.pointer_);
        return NULL;
    }
    return helper.pointer_;
}


