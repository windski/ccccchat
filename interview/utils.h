#ifndef UTILS_H_
#define UTILS_H_

#include "core.h"

extern void setnonblocking(int fd);

// add the file description to the epoll events list.
extern void addfd(int epollfd, int fd);

extern void removefd(int epollfd, int fd);

#endif
