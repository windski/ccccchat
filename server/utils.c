#include "utils.h"

int setnonblocking(int fd)
{
    int flag;
    if((flag = fcntl(fd, F_GETFL)) < 0) {
        perror("get fcntl failure");
        exit(-1);
    }

    flag |= O_NONBLOCK;

    if(fcntl(fd, F_SETFL) < 0) {
        perror("set fcntl failure");
        exit(-1);
    }

    return flag;
}

