#include "poll.h"

int poll_create(int counts_)
{
    for (int i = 0; i < _LIMIT; i++) {
        fds[i].fd = -1;
        fds[i].events = 0;
    }

    return poll(fds, counts_ + 1, -1);
}

int poll_registe_fd(int pollfd, int sockfd, int events)
{
    assert(pollfd > 0 && sockfd > 0);

    for (int i = 0; i < _LIMIT; i++) {
        if (fds[i].fd == -1) {
            fds[i].fd = sockfd;
            fds[i].events = events;
            fds[i].revents = 0;
            return 0;
        }
    }

    log_fatal("have no empty slot to registe.");
    return -1;
}

int poll_unregiste_fd(int pollfd, int sockfd)
{
    assert(pollfd > 0 && sockfd > 0);

    for (int i = 0; i < _LIMIT; i++) {
        if (fds[i].fd == sockfd) {
            fds[i].fd = -1;
            fds[i].events = 0;
            fds[i].revents = 0;

            return 0;
        }
    }

    log_fatal("should not doing here.");
    assert(0);

    return -1;
}

