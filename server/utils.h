#ifndef UTILS_H_
#define UTILS_H_

#include "comm_struct.h"

#define get_str_len(a, b) (((b) - (a)) / sizeof(char))

#ifndef ELEMSIZE
#define ELEMSIZE 32
#endif

extern int str_lt(char *l, char *r);
extern int str_bt(char *l, char *r);


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

extern int search_user(tree_head_t *user_table, pair_t *data);
extern char *get_kv_value(pair_t *data, char *target);

extern void insert_user(tree_head_t *user_table, pair_t *data);

#if defined (__linux__)
extern void encryption(char *target, int len);
#else
#define encryption(a, b) (printf("Please implement your own encryption algorithm..!"))
#endif


#endif
