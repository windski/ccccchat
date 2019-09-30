#include "poll.h"
#include "third_party/log.h"

int create_poll(int counts_)
{
    for (int i = 0; i < _LIMIT; i++) {
        fds[i].fd = -1;
        fds[i].events = 0;
    }

    return poll(fds, counts_ + 1, -1);
}

int registe_fd(int pollfd, int sockfd, int events)
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

    log_info("test");
    return -1;
}
