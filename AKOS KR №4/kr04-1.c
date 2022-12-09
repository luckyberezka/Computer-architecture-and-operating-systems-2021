#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {
    char *my_path = argv[1];
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);
    int fd = open(my_path, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd == -1) {
        exit(1);
    }
    if (truncate(my_path, (off_t)(rows * cols * (sizeof(double)))) != 0) {
        exit(1);
    }
    double *adress = mmap(0, rows * cols * (sizeof(double)), PROT_WRITE, MAP_SHARED, fd, 0);
    if (adress == MAP_FAILED) {
        exit(1);
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            adress[i * cols + j] = (double)((double )2*sin((double )i)) + (double)((double )4*cos((double )j/(double )2));
        }
    }
    if (munmap(adress, rows * cols * (sizeof(double))) == -1) {
        exit(1);
    }
}
