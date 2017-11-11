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
        char **target, int array_len)
{
    char *tmp = strchr(source, ch);
    char *tmp_sr = source;

    int i = 0;
    while(tmp) {
        if(i >= array_len) {
            // array_len = i;
            return -1;
        }

        strncpy(target[i], tmp_sr, get_str_len(tmp_sr, tmp));
        tmp_sr = tmp + 1;
        tmp = strchr(tmp_sr, ch);
        i++;
    }
    return i;
}


pair_t *make_kv_pair(char **info, int fir)
{
    if(fir % 2) {
        fputs("make info to pair failure..\n", stderr);
        return NULL;
    }

    pair_t *head = (pair_t *)malloc(sizeof(pair_t));

    head->length = fir / 2;

    struct elem *data = (struct elem *)malloc(sizeof(struct elem) * (head->length));

    for(int i = 0; i < fir; i += 2) {
        strncpy(data->key, info[i], sizeof(data->key));
        strncpy(data->value, info[i + 1], sizeof(data->value));
    }
    
    return head;
}


void free_kv_pair(pair_t *target)
{
    free(target->data);
    free(target);
}

