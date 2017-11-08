#ifndef UTILS_H_
#define UTILS_H_

#include "translatlib.h"

#define get_str_len(a, b) (((b) - (a)) / sizeof(char))

extern char **apply_2d_arr(int m, int n);
extern void free_2d_arr(char **target, int m);

extern int split_str(char *source, const char ch,
        char **target, int *array_len);

#endif
