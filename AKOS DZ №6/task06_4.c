#include <stdio.h>
#include <stdlib.h>

struct MyList {
    int value_;
    struct MyList *prev_;
};

struct MyList *Append(struct MyList *tail_list, int number) {
    struct MyList *tmp = malloc(sizeof(*tmp));
    if (!tmp) {
        while (tail_list) {
            struct MyList *trash = tail_list;
            tail_list = tail_list->prev_;
            free(trash);
        }
        perror("Fail malloc!");
        return tmp;
    }
    tmp->value_ = number;
    tmp->prev_ = tail_list;
    return tmp;
}

int main() {
    int val;
    struct MyList *lst = NULL;
    while (scanf("%d", &val) != EOF) {

        lst = Append(lst, val);
    }
    while (lst) {
        printf("%d\n", lst->value_);
        struct MyList *trash = lst;
        lst = lst->prev_;
        free(trash);
    }
    exit(0);
}

