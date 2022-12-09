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

int main(int argc, char *argv[]) {
    DIR *d = opendir(argv[1]);
    if (!d) {
        return 0;
    }
    int mysyze = strlen(argv[2]);
    struct dirent *ent;
    while ((ent = readdir(d))) {
        char path[PATH_MAX + 1];
        snprintf(path, sizeof(path), "%s/%s", argv[1], ent->d_name);
        struct stat buffer;
        int status = stat(path, &buffer);
        if ((status != -1) && (S_ISREG(buffer.st_mode))) {
            char nowbuffer[mysyze];
            int fd = open(path, O_RDONLY);
            ssize_t count = read(fd, nowbuffer, mysyze);
            if (count < mysyze) {
                close(fd);
                continue;
            }
            if (strncmp(argv[2], nowbuffer, mysyze) == 0) {
                printf("%s\n", ent->d_name);
            }
            close(fd);
        }
    }
    closedir(d);
}
