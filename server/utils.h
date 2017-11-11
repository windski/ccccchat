#ifndef UTILS_H_
#define UTILS_H_

#include "translatlib.h"

#define get_str_len(a, b) (((b) - (a)) / sizeof(char))

#ifndef ELEMSIZE
#define ELEMSIZE 32
#endif


struct elem{
    char key[ELEMSIZE];
    char value[ELEMSIZE];
};

struct pair {
    struct elem *data;
    int length;
};

typedef struct pair pair_t;

extern char **apply_2d_arr(int m, int n);
extern void free_2d_arr(char **target, int m);

extern int split_str(char *source, const char ch,
        char **target, int array_len);

extern pair_t *make_kv_pair(char **info, int fir);
extern void free_kv_pair(pair_t *target);

extern pair_t *parse_info(const char *msg);

#endif
