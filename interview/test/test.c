#include "../utils.h"
#include <stdbool.h>

void add_sock(int epollfd, int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLOUT | EPOLLET | EPOLLERR;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

bool read_once(int sockfd, char *buff, int len)
{
    ssize_t recv_size;
    bzero(buff, len);
    
    recv_size = recv(sockfd, buff, len, 0);
    if(recv_size == -1) {
        return false;
    } else if(recv_size == 0) {
        return false;
    }

    printf("read in %lu bytes from socket %d with content: %s\n", recv_size, sockfd, buff);

    return true;
}


bool write_bytes(int sockfd, const char *buff, int len)
{
    ssize_t send_size;
    printf("write out %d bytes to socket %d\n", len, sockfd);
    while(1) {
        send_size = send(sockfd, buff, len, 0);
        if(send_size < -1) {
            return false;
        } else if(send_size == 0) {
            return false;
        }

        len -= send_size;
        buff += send_size;
        if(len <= 0) {
            return true;
        }
    }
}


void make_connect(int epollfd, int nums, const char *ip, int port)
{
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);

    for(int i = 0; i < nums; i++) {
        /* sleep(1); */
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        printf("create a socket\n");
        if(sockfd < 0) {
            continue;
        }

        if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == 0) {
            printf("build connection %d\n", i);
            add_sock(epollfd, sockfd);
        }
    }
}


void close_connect(int epollfd, int fd)
{
    removefd(epollfd, fd);
    close(fd);
}


int main(int argc, const char *args[])
{
    char message[] = "12  get Makefile";

    if(argc <= 2) {
        fprintf(stderr, "usage: %s <ip> <port> <nums>\n", args[0]);
        exit(-1);
    }

    int epollfd = epoll_create(100);
    make_connect(epollfd, atoi(args[3]), args[1], atoi(args[2]));
    struct epoll_event events[_LIMIT];
    char buff[BUFFSIZE];

    while(1) {
        int num = epoll_wait(epollfd, events, _LIMIT, 2000);
        for(int i = 0; i < num; i++) {
            int sockfd = events[i].data.fd;

            if(events[i].events & EPOLLIN) {
                if(!read_once(sockfd, buff, BUFFSIZE)) {
                    close_connect(epollfd, sockfd);
                }

                struct epoll_event event;
                event.events = EPOLLOUT | EPOLLET | EPOLLERR;
                event.data.fd = sockfd;
                epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &event);
            } else if(events[i].events & EPOLLOUT) {
                if(!write_bytes(sockfd, message, strlen(message))) {
                    close_connect(epollfd, sockfd);
                }

                struct epoll_event event;
                event.events = EPOLLIN | EPOLLET | EPOLLERR;
                event.data.fd = sockfd;
                epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &event);
            } else if(events[i].events & EPOLLERR) {
                close_connect(epollfd, sockfd);
            }

        }
    }
    
    return 0;
}


