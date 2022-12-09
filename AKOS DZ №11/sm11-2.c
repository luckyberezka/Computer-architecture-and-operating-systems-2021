#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>


void PrintWord(long int from, long int to, const char *str) {
    for (long int i = from; i <= to; ++i) {
        printf("%c", str[i]);
    }
    printf("\n");
}

void PrintAnswer(long int start, long int end, const char *str) {
    long int left = -1, right = -1;
    for (long int i = end; i >= start; --i) {
        if (str[i] == '\n') {
            if (right == -1) {
                printf("\n");
            } else {
                PrintWord(left, right, str);
                left = -1;
                right = -1;
            }
        } else {
            if (right == -1) {
                right = i;
                left = i;
            } else {
                left = i;
            }
        }
    }
    if (str[start] == '\n') {
        printf("\n");
    } else if (right != -1) {
        PrintWord(start, right, str);
    }
}

bool SizeOfFile(const char *path, long unsigned int *size) {
    struct stat helper;
    if (stat(path, &helper) == -1) {
        return false;
    }
    *size = helper.st_size;
    return true;
}

bool IsValid(const char *str, long int *res) {
    int i = 0;
    long long int number = 0;
    while (str[i] != '\0') {
        if (str[i] == '+') {
            if (i != 0) {
                return false;
            } else {
                ++i;
                continue;
            }
        } else if (str[i] < '0' || str[i] > '9') {
            return false;
        }
        long long int old = number;
        number *= 10;
        if (number < old) {
            return false;
        }
        number += str[i] - 48;
        if (number < old) {
            return false;
        }
        ++i;
    }
    if (number >= LONG_MAX || number == 0) {
        return false;
    }
    *res = number;
    return true;
}

int main(int argc, char *argv[]) {
    char *my_path = argv[1];
    long int line1 = 0, line2 = 0;
    if (!IsValid(argv[2], &line1) || !IsValid(argv[3], &line2)) {
        perror("Invalid arguments!\n");
        exit(1);
    }

    long unsigned int size_of_file = 0;

    if (!SizeOfFile(my_path, &size_of_file)) {
        perror("Bad file!\n");
        exit(1);
    }

    if (size_of_file == 0) {
        return 0;
    }


    int fd = open(my_path, O_RDONLY);
    if (fd == -1) {
        perror("Bad file!\n");
        exit(1);
    }

    if (line1 >= line2) {
        exit(0);
    }


    long long int counter = 1;
    char *adress = mmap(0, size_of_file, PROT_READ, MAP_SHARED, fd, 0);
    if (adress == MAP_FAILED) {
        perror("Map fail!");
        exit(1);
    }

    long int index_for_start = -1, index_for_end = -1;

    bool flag_of_start = false;
    for (long int i = 0; i < size_of_file; ++i) {
        if (counter == line1) {
            if (!flag_of_start) {
                index_for_start = i;
            }
            flag_of_start = true;
        }
        if (adress[i] == '\n') {
            counter++;
            if (counter <= line2) {
                index_for_end = i - 1;
            } else {
                break;
            }
        }
        if (i + 1 == size_of_file && adress[i] != '\n') {
            counter++;
            if (counter <= line2) {
                index_for_end = i;
            }
        }
    }

    if (!flag_of_start) {
        if (munmap(adress, size_of_file) == -1) {
            perror("Trouble with munmap");
            exit(1);
        }
        if (close(fd) == -1) {
            perror("Trouble with close file");
            exit(1);
        }
        exit(0);
    }

    PrintAnswer(index_for_start, index_for_end, adress);

    if (munmap(adress, size_of_file) == -1) {
        perror("Trouble with munmap");
        exit(1);
    }
    if (close(fd) == -1) {
        perror("Trouble with close file");
        exit(1);
    }
}
