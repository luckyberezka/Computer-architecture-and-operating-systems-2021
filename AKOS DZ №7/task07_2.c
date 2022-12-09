#include <stdio.h>
#include <stdlib.h>

struct MyList {
    int value_;
    int counter;
    struct MyList *prev_;
    struct MyList *next_;
};

void Append(struct MyList **tail_list, int number) {
    struct MyList *tmp = malloc(sizeof(*tmp));
    tmp->value_ = number;
    tmp->counter = 1;
    tmp->prev_ = *tail_list;
    tmp->next_ = NULL;
    if (*tail_list) {
        (*tail_list)->next_ = tmp;
    }
    *tail_list = tmp;
}

struct MyList *Erase(struct MyList **tail_list, struct MyList **head_list, int number) {
    struct MyList *result = NULL;
    struct MyList *help_ptr = *tail_list;
    while (help_ptr) {
        if (help_ptr->value_ == number) {
            result = help_ptr;
            break;
        }
        help_ptr = help_ptr->prev_;
    }
    if (!result) {
        return result;
    }
    if (!result->next_) {
        *tail_list = result->prev_;
    } else {
        result->next_->prev_ = result->prev_;
    }
    if (!result->prev_) {
        *head_list = result->next_;
    } else {
        result->prev_->next_ = result->next_;
    }
    return result;
}

int main() {
    int val;
    struct MyList *lst = NULL;
    struct MyList *head = NULL;
    while (scanf("%d", &val) != EOF) {
        struct MyList *btw = Erase(&lst, &head, val);
        if (btw) {
            btw->counter++;
            btw->next_ = NULL;
            btw->prev_ = lst;
            if (lst) {
                lst->next_ = btw;
            }
            lst = btw;
        } else {
            Append(&lst, val);
        }
        if (!head) {
            head = lst;
        }
    }
    while (head) {
        printf("%d ", head->value_);
        printf("%d\n", head->counter);
        struct MyList *trash = head;
        head = head->next_;
        free(trash);
    }
    exit(0);
}

