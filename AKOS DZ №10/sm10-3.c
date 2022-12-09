#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    long long answ = 0;
    DIR *d = opendir(argv[1]);
    if (!d) {
        printf("%lld\n", answ);
        return 0;
    }
    struct dirent *ent;
    while ((ent = readdir(d))) {
        
        
        
        
        char path[PATH_MAX + 1];
        int i = 0;
        while (argv[1][i] != '\0') {
            path[i] = argv[1][i];
            ++i;
        }
        path[i] = '/';
        ++i;
        int j = 0;
        while (ent->d_name[j] != '\0') {
            path[i] = ent->d_name[j];
            ++i;
            ++j;
        }
        path[i] = '\0';
        
        
        
        struct stat buffer;
        int status = stat(path, &buffer);
        if ((status != -1) && (ent->d_name[0] >= 'A' && ent->d_name[0] <= 'Z') && (buffer.st_uid == getuid()) &&
            (S_ISREG(buffer.st_mode))) {
            answ += buffer.st_size;
        }
    }
    printf("%lld\n", answ);
}

