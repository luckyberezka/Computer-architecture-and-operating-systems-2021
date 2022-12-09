#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct MyArray {
    size_t sz_;
    size_t capacity_;
    char *pointer_;
} array = {};

void Append(struct MyArray *arr, char elem) {
    if (arr->sz_ + 1 > arr->capacity_) {
        //__builtin_mul_overflow();
        size_t updatecap = 2 * (arr->capacity_ + 1);
        char *tmp = realloc(arr->pointer_, updatecap * sizeof(*tmp));
        arr->pointer_ = tmp;
        arr->capacity_ = updatecap;
    }
    arr->pointer_[arr->sz_++] = elem;
}

void fprint7(FILE *f, int64_t number) {
    char *result = realloc(NULL, sizeof(*result));
    struct MyArray helper;
    helper.capacity_ = 1;
    helper.sz_ = 0;
    helper.pointer_ = result;

    if (number >= -6) {
        int flag = 0;
        if (number < 0) {
            number *= (-1);
            flag = 1;
        }

        int64_t next_number = number;
        while (7 <= next_number) {
            char new_elem = (char)(48 + next_number % 7);
            Append(&helper, new_elem);
            next_number = next_number / 7;
        }
        char new_elem = (char)(48 + next_number);
        Append(&helper, new_elem);
        if (flag == 1) {
            Append(&helper, '-');
        }


        char *array_for_end = realloc(NULL, sizeof(*array_for_end));
        struct MyArray ender;
        ender.capacity_ = 1;
        ender.sz_ = 0;
        ender.pointer_ = array_for_end;

        for (int i = helper.sz_ - 1; i >= 0; --i) {
            Append(&ender, helper.pointer_[i]);
        }
        Append(&ender, '\0');
        fprintf(f, "%s", ender.pointer_);
        free(helper.pointer_);
        free(ender.pointer_);
    } else {
        int64_t next_number = number;
        char new_elem = (char)(48 + (next_number % 7) * (-1));
        Append(&helper, new_elem);
        next_number = next_number / (-7);


        while (7 <= next_number) {
            char new_elem = (char) (48 + next_number % 7);
            Append(&helper, new_elem);
            next_number = next_number / 7;
        }
        new_elem = (char) (48 + next_number);
        Append(&helper, new_elem);
        Append(&helper, '-');

        char *array_for_end = realloc(NULL, sizeof(*array_for_end));
        struct MyArray ender;
        ender.capacity_ = 1;
        ender.sz_ = 0;
        ender.pointer_ = array_for_end;

        for (int i = helper.sz_ - 1; i >= 0; --i) {
            Append(&ender, helper.pointer_[i]);
        }
        Append(&ender, '\0');
        fprintf(f, "%s", ender.pointer_);
        free(helper.pointer_);
        free(ender.pointer_);
    }
}
