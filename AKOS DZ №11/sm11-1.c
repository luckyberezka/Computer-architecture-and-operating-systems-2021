#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    char *my_path = argv[1];
    int rows = atoi(argv[2]);
    int cols = atoi(argv[3]);
    int fd = open(my_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (truncate(my_path, (off_t)(rows * cols * (sizeof(int)))) != 0) {
        perror("!");
    }
    int *adress = mmap(0, rows * cols * (sizeof(int)), PROT_WRITE, MAP_SHARED, fd, 0);

    int matrix[rows][cols];
    int status[rows][cols];
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            status[i][j] = 0;
        }
    }
    // 0 == right
    // 1 == down
    // 2 == left
    // 3 == up

    int flag = 0, value = 1, now_i = 0, now_j = 0;
    while (value != rows * cols + 1) {
        matrix[now_i][now_j] = value++;
        status[now_i][now_j] = 1;
        if (flag == 0) {
            if ((now_j + 1 >= cols) || (status[now_i][now_j + 1] == 1)) {
                flag = 1;
                now_i++;
            } else {
                now_j++;
            }
        } else if (flag == 1) {
            if ((now_i + 1 >= rows) || (status[now_i + 1][now_j] == 1)) {
                flag = 2;
                now_j--;
            } else {
                now_i++;
            }
        } else if (flag == 2) {
            if ((now_j - 1 < 0) || (status[now_i][now_j - 1] == 1)) {
                flag = 3;
                now_i--;
            } else {
                now_j--;
            }
        } else {
            if ((now_i - 1 < 0) || (status[now_i - 1][now_j] == 1)) {
                flag = 0;
                now_j++;
            } else {
                now_i--;
            }
        }
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            adress[i * cols + j] = matrix[i][j];
        }
    }
    munmap(adress, rows * cols * (sizeof(int)));
}
