#include <stdlib.h>
#include <stdio.h>

struct dag_node {
    struct dag_node *left, *right;
    void *data;
};

struct MyArray {
    size_t sz_;
    size_t capacity_;
    struct dag_node **pointer_;
} array = {};

void Append(struct MyArray *arr, struct dag_node *elem) {
    if (arr->sz_ + 1 > arr->capacity_) {
        size_t updatecap = 2 * (arr->capacity_ + 1);
        struct dag_node **tmp = realloc(arr->pointer_, updatecap * sizeof(struct dag_node *));
        arr->pointer_ = tmp;
        arr->capacity_ = updatecap;
    }
    arr->pointer_[arr->sz_++] = elem;
}

struct Status {
    int flag;
};

void TakeArray(struct dag_node *root, struct MyArray *arr, struct Status *ch) {
    if (root == NULL || root->data == ch) {
        return;
    }
    root->data = ch;
    Append(arr, root);
    TakeArray(root->left, arr, ch);
    TakeArray(root->right, arr, ch);
}

void free_dag(struct dag_node *root) {
    struct dag_node **result = realloc(NULL, sizeof(struct dag_node *));
    struct MyArray helper;
    helper.capacity_ = 1;
    helper.sz_ = 0;
    helper.pointer_ = result;

    struct Status *checker = malloc(sizeof(struct Status));
    TakeArray(root, &helper, checker);
    for (size_t i = 0; i < helper.sz_; ++i) {
        free(helper.pointer_[i]);
    }
    free(helper.pointer_);
    free(checker);
}

