#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdalign.h>

typedef void (*finalizer_t)(void *ptr, size_t size);

const unsigned int step = sizeof(void *);
const unsigned int ravnina = alignof(void *);

struct allocation {
    struct allocation *prev;
    struct allocation *next;
    void *ptr;
    size_t size;
    finalizer_t finalizer;
    bool alive;
};

struct allocation *all_alloc = NULL;
void **stack_bottom;

bool points_to(void *ptr, struct allocation *A) {
    uintptr_t uptr = (uintptr_t)ptr, aptr = (uintptr_t)A->ptr;
    return (uptr >= aptr) && (uptr - aptr <= A->size);
}

void helper_func(void *ptr_for_check);

void podruchnii(struct allocation *alloc_for_check) {
    if (alloc_for_check->alive) {
        return;
    }
    alloc_for_check->alive = true;
    void **starter = alloc_for_check->ptr;
    void **border = alloc_for_check->ptr + alloc_for_check->size - alloc_for_check->size % alignof(void *);
    while (starter < border) {
        helper_func(*starter);
        ++starter;
    }
}

void helper_func(void *ptr_for_check) {
    if (!ptr_for_check) {
        return;
    }
    struct allocation *karakatica = all_alloc;
    while (karakatica) {
        if (points_to(ptr_for_check, karakatica)) {
            podruchnii(karakatica);
            return;
        }
        karakatica = karakatica->prev;
    }
}

void gc_init(char **argv) {
    stack_bottom = (void **)argv;
}
void *gc_malloc(size_t size, finalizer_t finalizer) {
    void *try_to_make = malloc(size);
    if (!try_to_make) {
        return try_to_make;
    }
    struct allocation *new_alloc = malloc(sizeof(*new_alloc));
    new_alloc->size = size;
    new_alloc->ptr = try_to_make;
    new_alloc->alive = false;
    new_alloc->prev = all_alloc;
    new_alloc->next = NULL;
    new_alloc->finalizer = finalizer;
    if (all_alloc) {
        all_alloc->next = new_alloc;
    }
    all_alloc = new_alloc;
    return try_to_make;
}
void gc_collect_impl(uintptr_t stack_top) {
    if (!all_alloc) {
        return;
    }
    struct allocation *now_alloc = all_alloc;
    while (now_alloc) {
        now_alloc->alive = false;
        now_alloc = now_alloc->prev;
    }
    if (stack_top % ravnina != 0) {
        stack_top += ravnina - (stack_top % ravnina);
    }
    void **begunok = (void **)stack_top;
    while (begunok < stack_bottom) {
        void *check_this_ptr = *begunok;
        helper_func(check_this_ptr);
        ++begunok;
    }
    now_alloc = all_alloc;
    while (now_alloc) {
        struct allocation *trash = now_alloc;
        now_alloc = now_alloc->prev;
        if (!trash->alive) {
            if (trash->prev) {
                trash->prev->next = trash->next;
            }
            if (trash->next) {
                trash->next->prev = trash->prev;
            }
            if (trash == all_alloc) {
                all_alloc = trash->prev;
            }
            if (trash->finalizer != NULL) {
                trash->finalizer(trash->ptr, trash->size);
            }
            if (trash->ptr) {
                free(trash->ptr);
            }
            free(trash);
        }
    }
}


void some_func(void* ptr, size_t sz) {
    printf("first object dead :c\n");
}

void some_func2(void* ptr, size_t sz) {
    printf("second object dead :c\n");
}

void some_func3(void* ptr, size_t sz) {
    printf("third object dead :c\n");
}

void gc_collect();

int main(int argc, char** argv) {
    gc_init(argv);
    gc_malloc(sizeof(int), some_func);
    gc_malloc(sizeof(int), some_func2);
    gc_malloc(sizeof(int), some_func3);
    gc_collect();
}
