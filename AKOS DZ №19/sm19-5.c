#include <stddef.h>
#include <pthread.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

int main(int argc, char *argv[]) {
    int fd0[2];
    if (pipe(fd0)) {
        return 0;
    }
    int fd1[2];
    if (pipe(fd1)) {
        return 0;
    }

    pid_t first_son = fork();
    if (!first_son) {
        close(fd1[0]);
        close(fd1[1]);
        close(fd0[0]);

        FILE *f = fopen(argv[1], "r");

        int sum = 0;
        int now_numb;
        while (fscanf(f, "%d", &now_numb) != EOF) {
            sum += now_numb;
        }

        if (write(fd0[1], &sum, sizeof(sum)) < 0) {
            close(fd0[1]);
            fclose(f);
            _exit(0);
        }

        close(fd0[1]);
        fclose(f);
        _exit(0);
    }

    pid_t second_son = fork();
    if (!second_son) {
        close(fd0[0]);
        close(fd0[1]);
        close(fd1[0]);

        FILE *f = fopen(argv[1], "r");

        int sum = 0;
        int now_numb;
        while (fscanf(f, "%d", &now_numb) != EOF) {
            sum += now_numb;
        }

        if (write(fd1[1], &sum, sizeof(sum)) < 0) {
            close(fd1[1]);
            fclose(f);
            _exit(0);
        }

        close(fd1[1]);
        fclose(f);
        _exit(0);
    }

    close(fd0[1]);
    close(fd1[1]);

    int status;
    waitpid(second_son, &status, 0);
    
    int sec_sum;
    if (read(fd1[0], &sec_sum, sizeof(sec_sum)) < 0) {
        return 0;
    }
    printf("%d\n", sec_sum);
    fflush(stdout);
    
    waitpid(first_son, &status, 0);
    int fir_sum;
    if (read(fd0[0], &fir_sum, sizeof(fir_sum)) < 0) {
        return 0;
    }
    printf("%d\n", fir_sum);
    fflush(stdout);

    printf("%d\n", fir_sum + sec_sum);
    fflush(stdout);
}
