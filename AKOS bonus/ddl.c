#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

enum {
    SecInTwoWeek = 604800 * 2,
    MonDelta = 1,
    YearDelta = 1900
};

int main() {
    struct tm now;
    struct tm *debbbug;
    while (scanf("%04d/%02d/%02d",
                 &now.tm_year, &now.tm_mon, &now.tm_mday) == 3) {
        now.tm_isdst = -1;
        now.tm_year -= 1900;
        now.tm_mon -= 1;
        now.tm_sec = 0;
        now.tm_min = 0;
        now.tm_hour = 0;

        time_t helper = mktime(&now);
        debbbug = localtime(&helper);
        if (debbbug->tm_wday != 5) {
            printf("ERROR\n");
            continue;
        }
        helper += SecInTwoWeek - 1;
        debbbug = localtime(&helper);
        printf("%04d/%02d/%02d %02d:%02d:%02d\n", debbbug->tm_year + YearDelta, debbbug->tm_mon + MonDelta,
               debbbug->tm_mday, debbbug->tm_hour, debbbug->tm_min, debbbug->tm_sec);
    }
}
