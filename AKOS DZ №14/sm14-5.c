#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <stdlib.h>
#include <sys/signalfd.h>

void Worker(int fd[2], int N, int whoami) {
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    int sfd = signalfd(-1, &mask, 0);

    FILE *help_instream = fdopen(fd[0], "rt");
    FILE *help_outstream = fdopen(fd[1], "wt");

    pid_t brother;

    if (whoami == 1) {
        if (fscanf(help_instream, "%d", &brother) <= 0) {
            perror("something wrong with pipe...");
            close(sfd);
            close(fd[0]);
            close(fd[1]);
            exit(1);
        }
        fprintf(help_outstream, "%d\n", getpid());
        fflush(help_outstream);
        kill(brother, SIGUSR1);

    } else if (whoami == 2) {
        fprintf(help_outstream, "%d\n", getpid());
        fflush(help_outstream);

        struct signalfd_siginfo myinfo;
        if (read(sfd, &myinfo, sizeof(struct signalfd_siginfo)) != sizeof(struct signalfd_siginfo)) {
            perror("something wrong with signalfd...");
            close(sfd);
            close(fd[0]);
            close(fd[1]);
            exit(1);
        }
        if (myinfo.ssi_signo == SIGUSR1) {
            if (fscanf(help_instream, "%d", &brother) <= 0) {
                perror("something wrong with pipe...");
                close(sfd);
                close(fd[0]);
                close(fd[1]);
                exit(1);
            }
        } else {
            _exit(1);
        }

        fprintf(help_outstream, "%d\n", 1);
        fflush(help_outstream);
        kill(brother, SIGUSR1);
    }

    while (1) {
        struct signalfd_siginfo myinfo;
        if (read(sfd, &myinfo, sizeof(struct signalfd_siginfo)) != sizeof(struct signalfd_siginfo)) {
            perror("something wrong with signalfd...");
            close(sfd);
            close(fd[0]);
            close(fd[1]);
            exit(1);
        }
        if (myinfo.ssi_signo == SIGUSR1) {
            int number;
            if (fscanf(help_instream, "%d", &number) <= 0) {
                perror("something wrong with signalfd...");
                close(sfd);
                close(fd[0]);
                close(fd[1]);
                exit(1);
            }
            if (number == N + 1) {
                break;
            }
            printf("%d %d\n", whoami, number);
            fflush(stdout);
            fprintf(help_outstream, "%d\n", number + 1);
            fflush(help_outstream);
            kill(brother, SIGUSR1);
            if (number == N) {
                break;
            }
        }
    }
    close(sfd);
    close(fd[0]);
    close(fd[1]);
    exit(0);

}

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);

    if (N == 0) {
        return 0;
    }

    int fd[2];
    if (pipe(fd)) {
        perror("pipec!");
        return 1;
    }
    pid_t first_son = fork();
    if (first_son == 0) {
        Worker(fd, N, 1);
    }
    pid_t second_son = fork();
    if (second_son == 0) {
        Worker(fd, N, 2);
    }
    close(fd[0]);
    close(fd[1]);
    wait(0);
    wait(0);
}

