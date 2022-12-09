#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        struct stat buffer;
        int status = stat(argv[i], &buffer);
        if (status == -1) {
            status = lstat(argv[i], &buffer);
            if (status == -1) {
                printf("%s (missing)\n", argv[i]);
            } else {
                printf("%s (broken symlink)\n", argv[i]);
            }
        } else {
            printf("%s\n", argv[i]);
        }
    }
}
//
