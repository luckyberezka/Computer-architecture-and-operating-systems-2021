#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/signalfd.h>
#include <inttypes.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>

struct MyArray {
    size_t sz_;
    size_t capacity_;
    char **pointer_;
} array = {};

void Append(struct MyArray *arr, char *elem) {
    if (arr->sz_ + 1 > arr->capacity_) {
        size_t updatecap = 2 * (arr->capacity_ + 1);
        char **tmp = realloc(arr->pointer_, updatecap * sizeof(*tmp));
        arr->pointer_ = tmp;
        arr->capacity_ = updatecap;
    }
    arr->pointer_[arr->sz_++] = elem;
}

int main(int argc, char *argv[]) {
    char **arr = realloc(NULL, sizeof(*arr));

    struct MyArray my_argv;
    my_argv.sz_ = 0;
    my_argv.capacity_ = 1;
    my_argv.pointer_ = arr;

    char **parr = realloc(NULL, sizeof(*arr));
    struct MyArray pmy_argv;
    pmy_argv.sz_ = 0;
    pmy_argv.capacity_ = 1;
    pmy_argv.pointer_ = parr;

    int flag = 0;
    int pos = -1;
    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], ";") == 0) {
            pos = i + 1;
            flag = 1;
            continue;
        }
        if (flag == 0) {
            Append(&my_argv, argv[i]);
        } else if (i != pos) {
            Append(&pmy_argv, argv[i]);
        }
    }
    pid_t first_son = fork();
    if (first_son == 0) {
        execvp(argv[1], my_argv.pointer_);
        _exit(127);
    }
    pid_t second_son = fork();
    if (second_son == 0) {
        execvp(argv[pos], pmy_argv.pointer_);
        _exit(127);
    }
    pid_t goddd = wait(0);
    if (goddd == first_son) {
        kill(second_son, SIGKILL);
    } else {
        kill(first_son, SIGKILL);
    }
    wait(0);
    free(my_argv.pointer_);
    fread(pmy_argv.pointer_);
}
