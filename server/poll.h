#ifndef POLL_H_
#define POLL_H_

#include "core.h"

#include <poll.h>

static struct pollfd fds[_LIMIT];

extern int create_poll(int counts_);

extern int registe_fd(int pollfd, int sockfd, int events);


#endif
