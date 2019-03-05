#ifndef SERVICE_H_
#define SERVICE_H_

#include "core.h"
#include "thread_pool.h"

typedef struct usergroup {
    int usernum;
    int *userids;
} ugroup_t;

int ugroup_create(int num);


#endif
