#include "core.h"
#include "socket.h"
#include <sys/epoll.h>


void setnonblocking(int fd)
{
    int flags;

    if((flags = fcntl(fd, F_GETFL)) < 0) {
        perror("fcntl: get the flags failure");
        return ;
    }

    flags |= O_NONBLOCK;

    if((flags = fcntl(fd, F_SETFL)) < 0) {
        perror("fcntl: set the flags failure");
        return ;
    }
}


// add the file description to the epoll events list.
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

int main(int argc, const char *args[])
{
    if(argc < 2) {
        fprintf(stderr, "usage: %s -p <port>\n", args[0]);
        exit(-1);
    }

    struct epoll_event events[_LIMIT];
    int sockfd = create_socket(NULL, atoi(args[1]));
    make_listen(sockfd, 1024);

    int epollfd = epoll_create(10);
    if(epollfd < 0) {
        perror("epoll_create failure");
        exit(-2);
    }

    addfd(epollfd, sockfd);

    while(1) {
        int epollnum = epoll_wait(epollfd, events, _LIMIT, -1);
        if(epollnum < 0) {
            perror("epoll_wait: failure");
            break;
        }

        for(int i = 0; i < epollnum; i++) {

            if(events[i].data.fd == sockfd && events[i].events & EPOLLIN) {
                // SO, a new connection..
                struct sockaddr_in cliaddr;
                socklen_t socklen = sizeof(cliaddr);

                int newfd = accept(sockfd, (struct sockaddr *)&cliaddr, &socklen);
                assert(newfd > 0);

                addfd(epollfd, newfd);
                printf("new connection\n");
            } else if(events[i].events & EPOLLIN) {
                // client send the data need to handle.
                char buff[BUFFSIZE];
                bzero(buff, sizeof(buff));

                // receive 4 bytes data, firstly.
                ssize_t count = recv(events[i].data.fd, buff, 4, 0);
                if(count < 0) {
                    perror("recv: failure");
                    break;
                } else if(count == 0) {
                    // TODO: remove fd from epollfd.
                    close(events[i].data.fd);
                }
                printf("count: %s\n", buff);

                count = atoi(buff);
                count = recv(events[i].data.fd, buff, count, 0);

                printf("buff: %s\n", buff);
            }
        }
    }

    return 0;
}

