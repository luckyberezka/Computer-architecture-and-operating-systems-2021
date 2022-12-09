#include <stdio.h>

void TakeNextNum(const char *v, int *ind_for_start, int *next_numb_ind, int *size_of_numb) {
    int number_find = 0;
    int last_find_zero = -1;
    while (v[*ind_for_start] != '\0') {
        if (v[*ind_for_start] >= '0' && v[*ind_for_start] <= '9') {
            if (number_find == 1) {
                *size_of_numb = *size_of_numb + 1;
            } else {
                if (v[*ind_for_start] != '0') {
                    *next_numb_ind = *ind_for_start;
                    *size_of_numb = *size_of_numb + 1;
                    number_find = 1;
                } else {
                    last_find_zero = (int) *ind_for_start;
                }
            }
        } else {
            if (number_find == 1) {
                return;
            }
        }
        *ind_for_start = *ind_for_start + 1;
    }
    if (number_find == 0 && last_find_zero != -1) {
        *next_numb_ind = last_find_zero;
        *size_of_numb = 1;
    }
}

int CompareForNumber(const char *v1, int f_ind, int f_size, const char *v2, int s_ind, int s_size) {
    if (f_size != s_size) {
        if (f_size < s_size) {
            return -1;
        } else {
            return 1;
        }
    }
    for (int i = 0; i < f_size; ++i) {
        if (v1[f_ind + i] < v2[s_ind + i]) {
            return -1;
        }
        if (v1[f_ind + i] > v2[s_ind + i]) {
            return 1;
        }
    }
    return 0;
}

int vercmp(const char *v1, const char *v2) {
    int first_ind_numb = -1;
    int first_nub_size = 0;
    int first_start_point = 0;

    int second_ind_numb = -1;
    int second_nub_size = 0;
    int second_start_point = 0;

    TakeNextNum(v1, &first_start_point, &first_ind_numb, &first_nub_size);
    TakeNextNum(v2, &second_start_point, &second_ind_numb, &second_nub_size);

    while (first_nub_size != 0 && second_nub_size != 0) {
        int now_res = CompareForNumber(v1, first_ind_numb, first_nub_size, v2, second_ind_numb, second_nub_size);
        if (now_res != 0) {
            return now_res;
        }

        first_ind_numb = -1;
        first_nub_size = 0;

        second_ind_numb = -1;
        second_nub_size = 0;

        TakeNextNum(v1, &first_start_point, &first_ind_numb, &first_nub_size);
        TakeNextNum(v2, &second_start_point, &second_ind_numb, &second_nub_size);
    }

    if (first_nub_size == 0 && second_nub_size == 0) {
        return 0;
    } else if (first_nub_size == 0 && second_nub_size != 0) {
        return -1;
    } else {
        return 1;
    }
}
