#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int64_t generate_or_fail(size_t i);

int64_t* parallel_populate(size_t N) {
    int64_t *res = mmap(0, N * sizeof(int64_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    pid_t threads[N];

    for (int i = 0; i < N; ++i) {
        threads[i] = fork();
        if (threads[i] == 0) {
            res[i] = generate_or_fail(i);
            exit(0);
        }
    }

    for (int i = 0; i < N; ++i) {
        if (threads[i] == -1) {
            res[i] = -1;
        } else {
            int podruchnii;
            waitpid(threads[i], &podruchnii, 0);
            if (WEXITSTATUS(podruchnii) == 13) {
                res[i] = -1;
            }
        }
    }

    return res;
}
