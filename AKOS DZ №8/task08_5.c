#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

typedef double (*funcptr_t)(double);

struct FunctionTable {
    char* name;
    funcptr_t func;
};

struct FunctionTable helper[] = {
        {"sin", sin},
        {"cos", cos},
        {"exp", exp},
        {"log", log},
        {"tan", tan},
        {"sqrt", sqrt}
};

int MyCompare(const void *val1, const void *val2) {
    struct FunctionTable *first = (struct FunctionTable *) val1;
    struct FunctionTable *second = (struct FunctionTable *) val2;
    return strcmp(first->name, second->name);
}

int main() {
    qsort(helper, sizeof(helper) / sizeof(struct FunctionTable), sizeof(struct FunctionTable), MyCompare);
    char *now_func;
    double arg;
    while (scanf("%ms%lf", &now_func, &arg) != EOF) {
        struct FunctionTable find_it = {now_func, NULL};
        struct FunctionTable *elem = (struct FunctionTable *) bsearch(&find_it, helper,
                sizeof(helper) / sizeof(struct FunctionTable), sizeof(struct FunctionTable), MyCompare);
        if (elem == NULL) {
            printf("nan\n");
        } else {
            printf("%lf\n", elem->func(arg));
        }
        free(now_func);
    }
}
