#include <stdio.h>
#include <dlfcn.h>

int main(int argc, char *argv[]) {
    double (*func)(double);
    void *helper = dlopen("libm.so.6", RTLD_LAZY);
    func = (double (*)(double)) dlsym(helper, argv[1]);
    double arg;
    while (scanf("%lf", &arg) != EOF) {
        printf("%.10g\n", (*func)(arg));
    }
    dlclose(helper);
}

