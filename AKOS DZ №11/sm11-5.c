#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>

#define min(x, y) ( (x) < (y) ? (x) : (y) )

int main(int argc, char **argv) {
    int fd = open(argv[1], O_RDONLY);
    struct stat my_file;
    if (fstat(fd, &my_file) == -1) {
        return 0;
    }
    if (my_file.st_size == 0) {
        close(fd);
        printf("0\n0\n0\n");
        return 0;
    }
    long long sum = 0;
    long long l = 0;
    long long r = my_file.st_size + 1;
    while (r > l) { //двоичным поиском ищем хорошую длину
        long long m = (r + l) / 2;
        void *adress = mmap(NULL, m, PROT_READ, MAP_PRIVATE, fd, 0);
        if (adress == MAP_FAILED) {
            r = m;
        } else {
            munmap(adress, m);
            l = m + 1;
        }
    } //в l получим оптимальный размер
    long long maximum = r - 1;
    l = r - 1;
    long long cur_index = 0;
    void* adress;
    char* data;
    long long counter;
    while (cur_index < my_file.st_size) { //пока индекс меньше размера
        l = (my_file.st_size - cur_index) < l ? my_file.st_size - cur_index : l;//смотрим сколько байтов хотим достать
        adress = mmap(0, l, PROT_READ, MAP_PRIVATE, fd, cur_index);
        data = adress;
        cur_index += l;
        counter = 0; //итератор для прохождения по куску памяти адрес
        while (counter < l) { //идем по куску памяти пока идется
            int32_t res = 0;
            unsigned int high_order_bit = 1;
            unsigned int shift = 0;
            unsigned char byte = 1;
            while (high_order_bit != 0) {
                if (counter >= l) {
                    munmap(adress, l);
                    l = (my_file.st_size - cur_index) < l ? my_file.st_size - cur_index : l;
                    if (l == 0) {
                        break;
                    }
                    adress = mmap(0, l, PROT_READ, MAP_PRIVATE, fd, cur_index);
                    cur_index += l;
                    data = adress;
                    counter = 0;
                }
                byte = data[counter];
                res |= ((uint32_t)(byte & 0x7f)) << shift;
                shift += 7;
                ++counter;
                high_order_bit = byte & 0x80;
            }
            if (shift < sizeof(int32_t) * 8 && (byte & 0x40) != 0) {
                res |= -(((uint32_t) 1) << shift);
            }
            sum += res;
        }
        if (l != 0) {
            munmap(adress, l);
        }
    }
    printf("%llu\n%lld\n%lld\n", my_file.st_size, maximum, sum);
    close(fd);
    return 0;
}
