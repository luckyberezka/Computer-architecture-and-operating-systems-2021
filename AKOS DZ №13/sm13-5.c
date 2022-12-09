#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);

    int fd0[2];
    int fd1[2];


    if (pipe(fd0)) {
        perror("pipec!");
        return 1;
    }

    if (pipe(fd1)) {
        perror("pipec!");
        return 1;
    }

    FILE *in_first_elem = fdopen(fd0[1], "wt");
    fprintf(in_first_elem, "%d\n", 1);
    fflush(in_first_elem);

    pid_t first_son = fork();
    if (first_son == -1) {
        perror("fork failed!");
        return 1;
    }
    if (first_son == 0) {
        int now;
        close(fd0[1]);
        FILE *help_instream = fdopen(fd0[0], "rt");
        FILE *help_outstream = fdopen(fd1[1], "wt");
        while (fscanf(help_instream, "%d", &now) == 1) {
            if (now == N) {
                break;
            }
            printf("1 %d\n", now);
            fflush(stdout);
            fprintf(help_outstream, "%d\n", now + 1);
            fflush(help_outstream);
        }
        _exit(0);
    }

    pid_t second_son = fork();
    if (second_son == -1) {
        perror("fork failed!");
        return 1;
    }
    if (second_son == 0) {
        int now;
        close(fd1[1]);
        FILE *help_instream = fdopen(fd1[0], "rt");
        FILE *help_outstream = fdopen(fd0[1], "wt");
        while (fscanf(help_instream, "%d", &now) == 1) {
            if (now == N) {
                break;
            }
            printf("2 %d\n", now);
            fflush(stdout);
            fprintf(help_outstream, "%d\n", now + 1);
            fflush(help_outstream);
        }
        _exit(0);
    }

    close(fd0[1]);
    close(fd1[1]);
    wait(0);
    wait(0);
    printf("Done\n");
}
