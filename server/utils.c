#include "utils.h"


char **apply_2d_arr(int m, int n)
{
    char **tmp = (char **)malloc(sizeof(char *) * m);

    for(int i = 0; i < m; i++) {
        tmp[i] = (char *)malloc(sizeof(char) * n);
    }
    return tmp;
}

void free_2d_arr(char **target, int m)
{
    for(int i = 0; i < m; i++) {
        free(target[i]);
    }
    free(target);
}

int split_str(char *source, const char ch,
        char **target, int *array_len)
{
    char *tmp = strchr(source, ch);
    char *tmp_sr = source;

    int i = 0;
    while(tmp) {
        if(i >= *array_len) {
            array_len = &i;
            return -1;
        }

        strncpy(target[i], tmp_sr, get_str_len(tmp_sr, tmp));
        tmp_sr = tmp + 1;
        tmp = strchr(tmp_sr, ch);
        i++;
    }
    return i;
}

