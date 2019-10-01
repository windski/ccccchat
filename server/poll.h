#ifndef POLL_H_
#define POLL_H_

#include "core.h"

#include <poll.h>

static struct pollfd fds[_LIMIT];

extern int poll_create(int counts_);

extern int poll_registe_fd(int pollfd, int sockfd, int events);

extern int poll_unregiste_fd(int pollfd, int sockfd);


#endif
