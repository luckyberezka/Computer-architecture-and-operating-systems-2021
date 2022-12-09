#include <unistd.h>

enum {
    BUF_SIZE = 4096,
};

ssize_t copy_file(int in_fd, int out_fd) {
    char buf[BUF_SIZE];
    if (in_fd < 0) {
        return -1;
    }
    if (out_fd < 0) {
        return -1;
    }
    ssize_t answer = 0;
    ssize_t result;
    while ((result = read(in_fd, buf, BUF_SIZE)) > 0) {
        ssize_t now;
        ssize_t counter = 0;
        while (counter != result) {
            now = write(out_fd, buf + counter, result - counter);
            if (now < 0) {
                return -1;
            }
            counter += now;
        }
        answer += counter;
    }
    return answer;
}
