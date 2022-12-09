#include <stdio.h>
#include <dlfcn.h>
#include <sys/stat.h>

const char *perms_to_str(char *buf, size_t size, int perms) {
    if (size == 0) {
        return buf;
    }
    char full_answer[9] = "---------";
    unsigned int checker = (unsigned int) perms;
    //checker = (checker << 24) >> 24;

    if (checker & S_IRUSR) {
        full_answer[0] = 'r';
    }
    if (checker & S_IWUSR) {
        full_answer[1] = 'w';
    }
    if (checker & S_IXUSR) {
        full_answer[2] = 'x';
    }


    if (checker & S_IRGRP) {
        full_answer[3] = 'r';
    }
    if (checker & S_IWGRP) {
        full_answer[4] = 'w';
    }
    if (checker & S_IXGRP) {
        full_answer[5] = 'x';
    }


    if (checker & S_IROTH) {
        full_answer[6] = 'r';
    }
    if (checker & S_IWOTH) {
        full_answer[7] = 'w';
    }
    if (checker & S_IXOTH) {
        full_answer[8] = 'x';
    }

    if ((checker & S_ISUID) && ((checker & S_IXGRP) || checker & S_IXOTH)) {
        full_answer[2] = 's';
    }

    if ((checker & S_ISGID) && (checker & S_IXOTH)) {
        full_answer[5] = 's';
    }

    if ((checker & S_ISVTX) && (checker & S_IWOTH) && (checker & S_IXOTH)) {
        full_answer[8] = 't';
    }

    buf[size - 1] = '\0';
    if (size > 1) {
        for (int i = 0; i < size - 1; ++i) {
            buf[i] = full_answer[i];
        }
    }

    return buf;
}

