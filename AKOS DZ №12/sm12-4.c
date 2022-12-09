#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>


int main(int argc, char *argv[]) {
    char *path = getenv("XDG_RUNTIME_DIR");
    if (!path) {
        path = getenv("TMPDIR");
        if (!path) {
            path = "/tmp";
        }
    }
    char file_path[PATH_MAX + 1];
    snprintf(file_path, sizeof(file_path), "%s/%ld.py", path, (long int)getpid());

    int fd = open(file_path, O_WRONLY | O_CLOEXEC | O_CREAT, 0700);
    FILE *stream = fdopen(fd, "w");
    char *pomogator = "#! /usr/bin/env python3\nimport os\nprint(";
    fprintf(stream, "%s", pomogator);
    for (int i = 1; i < argc; ++i) {
        fprintf(stream, "%s", argv[i]);
        if (i + 1 == argc) {
            fprintf(stream, "%s", ")\n");
        } else {
            fprintf(stream, "%s", "*");
        }
    }
    fprintf(stream, "os.remove(\"%s\")\n", file_path);
    fclose(stream);
    execlp(file_path, file_path, NULL);
}
