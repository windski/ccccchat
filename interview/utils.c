#include "utils.h"

void setnonblocking(int fd)
{
    int flags;

    if((flags = fcntl(fd, F_GETFL)) < 0) {
        perror("fcntl: get the flags failure");
        exit(-3);
    }

    flags |= O_NONBLOCK;

    if((fcntl(fd, F_SETFL)) < 0) {
        perror("fcntl: set the flags failure");
        exit(-4);
    }
}


void addfd(int epollfd, int fd)
{
    assert(epollfd > 0);
    assert(fd > 0);

    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}


void removefd(int epollfd, int fd)
{
    assert(epollfd > 0);
    assert(fd > 0);

    // There is a bug in Linux Kernel before 2.6.9, more details check it in man pages.
    // I don't care cuz....
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}

