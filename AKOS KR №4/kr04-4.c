#include <sys/mman.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>

bool NameCheck(char *name) {
    long long int length = 0;
    while (name[length] != '\0') {
        ++length;
    }
    if (length < 3) {
        return false;
    }
    if ((name[length - 3] != '.') || (name[length - 2] != 'p') || name[length - 1] != 'y') {
        return false;
    }
    return true;
}

void DirCheck(char *dir_path) {
    DIR *my_dir = opendir(dir_path);
    if (!my_dir) {
        return;
    }

    struct dirent *ent;
    while ((ent = readdir(my_dir))) {

        // проверяем .py
        if (!NameCheck(ent->d_name)) {
            continue;
        }

        //  строим полный путь к файлу
        char file_path[PATH_MAX + 1];
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, ent->d_name);

        // проверяем права на файл
        if (access(file_path, R_OK | X_OK) != 0) {
            continue;
        }

        // достаём путь к файлу-интерпритатору и проверяем на !#
        struct stat buffer;
        int status = stat(file_path, &buffer);
        if (status == -1 || buffer.st_size < 2) {
            continue;
        }



        char inter_path[PATH_MAX + 1];


        int fd = open(file_path, O_RDONLY);
        if (fd == -1) {
            continue;
        }

        bool inter_status = true;
        long long int i_p_i = 0;

        char *in_file = mmap(0, buffer.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
        if (in_file == MAP_FAILED) {
            close(fd);
            continue;
        }
        for (long long int i = 0; i < buffer.st_size; ++i) {
            if (i == 0) {
                if (in_file[i] != '#') {
                    inter_status = false;
                    break;
                }
            } else if (i == 1) {
                if (in_file[i] != '!') {
                    inter_status = false;
                    break;
                }
            } else {
                if (in_file[i] == '\0' || in_file[i] == '\t' || in_file[i] == '\n' || in_file[i] == ' ') {
                    break;
                } else {
                    inter_path[i_p_i] = in_file[i];
                    ++i_p_i;
                }
            }
        }
        inter_path[i_p_i] = '\0';
        if (!inter_status || i_p_i == 0) {
            continue;
        }
        munmap(in_file, buffer.st_size);
        close(fd);

        // проверяем файл-интерпритатор
        struct stat pomogator;
        int pokazatel = stat(inter_path, &pomogator);

        if ((pokazatel != -1) && S_ISREG(pomogator.st_mode) && (access(inter_path, X_OK) == 0)) {
            printf("%s\n", file_path);
        }
    }



    closedir(my_dir);
}
int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        DirCheck(argv[i]);
    }
}
