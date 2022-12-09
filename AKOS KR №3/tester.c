#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

void discrim(float a, float b, float c, float *res);

int main(int argc, char *argv[]) {
    float a = 2, b = 3, c = 6;
    float res;
    discrim(a, b, c, &res);
    printf("%f", res);
}
