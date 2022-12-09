#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

struct MyArray {
    int sz_;
    int capacity_;
    char **pointer_;
} array = {};

void Append(struct MyArray *arr, char *elem) {
    if (arr->sz_ + 1 > arr->capacity_) {
        //__builtin_mul_overflow();
        int updatecap = 2 * (arr->capacity_ + 1);
        char **tmp = realloc(arr->pointer_, updatecap * sizeof(*tmp));
        arr->pointer_ = tmp;
        arr->capacity_ = updatecap;
    }
    arr->pointer_[arr->sz_++] = elem;
}


int compare(const void *x1, const void *x2) {
    char **new_x1 = (char **) x1;
    char **new_x2 = (char **) x2;
    char *first_str = *new_x1;
    char *second_str = *new_x2;
    return strcasecmp(first_str, second_str);
}

void Inside(DIR *d, char *way) {

    char **result = realloc(NULL, sizeof(*result));
    struct MyArray helper;
    helper.capacity_ = 1;
    helper.sz_ = 0;
    helper.pointer_ = result;



    struct dirent *ent;
    while ((ent = readdir(d))) {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        char path[PATH_MAX + 1];
        snprintf(path, sizeof(path), "%s/%s", way, ent->d_name);



        struct stat buffer;
        int status = lstat(path, &buffer);
        if ((status != -1) && (S_ISDIR(buffer.st_mode))) {
            Append(&helper, strdup(ent->d_name));
        }
    }

    closedir(d);

    qsort(helper.pointer_, helper.sz_, sizeof(char *), compare);

    for (int i = 0; i < helper.sz_; ++i) {
        char superway[PATH_MAX + 1];
        snprintf(superway, sizeof(superway), "%s/%s", way, helper.pointer_[i]);

        DIR *new_dir = opendir(superway);
        
        if (new_dir) {
            
            printf("cd %s\n", helper.pointer_[i]);
            Inside(new_dir, superway);
            printf("cd ..\n");
        }
        
        free(helper.pointer_[i]);
    }

    free(helper.pointer_);

}

int main(int argc, char *argv[]) {
    DIR *d = opendir(argv[1]);
    if (!d) {
        return 0;
    }
    Inside(d, argv[1]);
}

