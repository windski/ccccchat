#include "service.h"

static ugroup_t *groups = NULL;

int ugroup_create(int num)
{
    if(num < 0) {
        return -1;
    }

    groups = (ugroup_t *)malloc(sizeof(ugroup_t) * num);
    assert(groups != NULL);

    for(int i = 0; i < num; i++) {
        groups[i].usernum = 0;
        groups[i].userids = (int *)malloc(sizeof(int) * _LIMIT);
        assert(groups[i].userids != NULL);
    }

    return 0;
}

