#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    struct tm prev, now;
    bool is_prev_empty = true;
    FILE *my_instream = fopen(argv[1], "r");
    while (fscanf(my_instream, "%04d/%02d/%02d %02d:%02d:%02d",
                  &now.tm_year, &now.tm_mon, &now.tm_mday, &now.tm_hour,
                  &now.tm_min, &now.tm_sec) == 6) {
        now.tm_isdst = -1;
        now.tm_year -= 1900;
        now.tm_mon -= 1;
        if (is_prev_empty) {
            prev = now;
            is_prev_empty = false;
            continue;
        }
        printf("%ld\n", (long int)(difftime(mktime(&now), mktime(&prev))));
        prev = now;
    }
    fclose(my_instream);
}
