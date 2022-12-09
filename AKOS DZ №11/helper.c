#include <sys/stat.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>



bool SizeOfFile(const char *path, long long int *size) {
    struct stat helper;
    if (stat(path, &helper) == -1) {
        return false;
    }
    *size = helper.st_size;
    return true;
}

long long int MyMin(long long int a, long long int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int main(int argc, char *argv[]) {
    char *path = argv[1];
    long long int size_of_file = 0;
    SizeOfFile(path, &size_of_file);
    long long int l = 0, r = size_of_file + 1;


    if (size_of_file == 0) {
        printf("0\n0\n0\n");
        return 0;
    }


    int fd = open(path, O_RDONLY);
    while (r > l) {
        long long int middle = (l + r) / 2;
        char *adress;
        if ((adress = mmap(0, middle, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
            r = middle;
        } else {
            l = middle + 1;
            munmap(adress, middle);
        }
    }

    printf("%lld\n%lld\n", size_of_file, r - 1);
    long long int step = r - 1;
    long long int delta = 0;
    long long int res = 0;

    int now_number = 0;
    int sft = 0;


    while (size_of_file > 0) {
        bool status = false;
        long long int now_size = MyMin(step, size_of_file);
        char *adress = mmap(0, now_size, PROT_READ, MAP_SHARED, fd, delta);
        if (adress == MAP_FAILED) {
            perror("MAP FAILED!");
            return 1;
        }
        for (long long int i = 0; i < now_size;) {
            unsigned int first_bit = 1;
            unsigned int next_seven;
            while (first_bit != 0) {
                if (i >= now_size) {
                    status = true;
                    break;
                }
                unsigned int byte = adress[i];
                next_seven = byte & 127;
                now_number |= next_seven << sft;
                sft += 7;
                first_bit = (byte & 128) >> 7;
                ++i;
            }
            if (!status) {
                if (sft < 32 && (next_seven & 64) != 0) {
                    now_number |= (0xffffffff << sft);
                }
                res += now_number;
                now_number = 0;
                sft = 0;
            }
        }

        size_of_file -= now_size;
        delta += step;
        munmap(adress, now_size);
    }
    printf("%lld\n", res);
}
