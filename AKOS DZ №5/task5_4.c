#include <unistd.h>

struct FileWriteState
{
    int fd;              // "файловый дескриптор", для вывода на стандартный поток вывода - 1
    unsigned char *buf;  // указатель на буфер
    int bufsize;         // размер буфера
    int index;
    // здесь потребуется добавить поля для реализации буферизованной записи
};

enum {
    BUF_SIZE = 4096,
};

static char buffer[BUF_SIZE];

struct FileWriteState helper = {1, &buffer, BUF_SIZE, 0};

struct FileWriteState *stout = &helper;

void flush(struct FileWriteState *out) {
    ssize_t now;
    ssize_t counter = 0;
    while (counter != out->index) {
        now = write(out->fd, out->buf + counter, out->index - counter);
        if (now < 0) {
            break;
        }
        counter += now;
    }
    out->index = 0;
}

void writechar(int c, struct FileWriteState *out) {
    out->buf[out->index] = c;
    ++out->index;
    if (out->index == out->bufsize) {
        flush(out);
    }
}
