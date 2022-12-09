#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    long long answ = 0;
    for (int i = 1; i < argc; ++i) {
        struct stat buffer;
        int status = lstat(argv[i], &buffer);
        if ((status != -1) && (!S_ISLNK(buffer.st_mode)) && (S_ISREG(buffer.st_mode)) && (buffer.st_nlink == 1)) {
            answ += buffer.st_size;
        }
    }
    printf("%lld", answ);
}
