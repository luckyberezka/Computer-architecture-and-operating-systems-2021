#include <stdio.h>
#include <stdlib.h>

struct Apex {
    int value_;
    struct Apex *left_son_;
    struct Apex *right_son_;
};

void Insert(struct Apex **root, int number) {
    if (!(*root)) {
        struct Apex *new_node = malloc(sizeof(*new_node));
        new_node->left_son_ = NULL;
        new_node->right_son_ = NULL;
        new_node->value_ = number;
        *root = new_node;
        return;
    }

    if (number < (*root)->value_) {
        if ((*root)->left_son_) {
            Insert(&((*root)->left_son_), number);
        } else {
            struct Apex *new_node = malloc(sizeof(*new_node));
            new_node->left_son_ = NULL;
            new_node->right_son_ = NULL;
            new_node->value_ = number;
            (*root)->left_son_ = new_node;
            return;
        }
    } else if (number > (*root)->value_) {
        if ((*root)->right_son_) {
            Insert(&((*root)->right_son_), number);
        } else {
            struct Apex *new_node = malloc(sizeof(*new_node));
            new_node->left_son_ = NULL;
            new_node->right_son_ = NULL;
            new_node->value_ = number;
            (*root)->right_son_ = new_node;
            return;
        }
    }
}

void DeleteTree(struct Apex *root) {
    if (!root) {
        return;
    }
    DeleteTree(root->right_son_);
    printf("%d ", root->value_);
    DeleteTree(root->left_son_);
    free(root);
}

int main() {
    int val;
    int flag = 0;
    struct Apex *now_root = NULL;
    while (scanf("%d", &val) != EOF) {
        flag = 1;
        if (val == 0) {
            DeleteTree(now_root);
            printf("0 ");
            now_root = NULL;
        } else {
            Insert(&now_root, val);
        }
    }
    if (now_root) {
        DeleteTree(now_root);
        printf("0 ");
    }
    if (flag == 1) {
        printf("\n");
    }
}
