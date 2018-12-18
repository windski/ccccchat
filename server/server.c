#define _GNU_SOURCE 1
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <assert.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFSIZE 4096
#define _LIMIT 65535

typedef struct {
    struct sockaddr_in useraddr;
    char *write_buff;
    char read_buff[BUFFSIZE];
} user_t;

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

int main(int argc, char *args[])
{
    if(argc != 2) {
        printf("usage: %s port number\n", args[0]);
        return 1;
    }

    struct sockaddr_in sockaddr;
    bzero(&sockaddr, sizeof(sockaddr));
    sockaddr.sin_port = htons(atoi(args[1]));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    if(bind(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) != 0) {
        printf("can't bind the address or port\n");
        return -1;
    }

    int ret = listen(sockfd, 1024);
    assert(ret != -1);

    user_t *users = (user_t *)malloc(sizeof(user_t) * _LIMIT);

    struct pollfd fds[_LIMIT + 1];
    int user_counter = 0;
    for(int i = 1; i <= _LIMIT; i++) {
        fds[i].fd = -1;
        fds[i].events = 0;
    }
    fds[0].fd = sockfd;
    fds[0].events = POLLIN | POLLERR;
    fds[0].revents = 0;

    while(1) {
        ret = poll(fds, user_counter + 1, -1);

        if(ret < 0) {
            printf("poll failure\n");
            break;
        }

        for(int i = 0; i < user_counter + 1; i++) {
            if((fds[i].fd == sockfd) && (fds[i].revents & POLLIN)) {
                struct sockaddr_in cliaddr;
                socklen_t socklen = sizeof(cliaddr);
                int connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &socklen);

                if(connfd < 0) {
                    printf("errno is: %d\n", errno);
                    continue;
                }

                if(user_counter >= _LIMIT) {
                    const char *info = "too many users\n";
                    printf("%s", info);
                    send(connfd, info, strlen(info), 0);
                    close(connfd);
                    continue;
                }

                user_counter++;
                users[connfd].useraddr = cliaddr;
                setnonblocking(connfd);

                fds[user_counter].fd = connfd;
                fds[user_counter].events = POLLIN | POLLRDHUP | POLLERR;
                fds[user_counter].revents = 0;
                printf("comes a new user, now have %d users\n", user_counter);
            } else if(fds[i].revents & POLLERR) {
                printf("get an error from %d\n", fds[i].fd);
                char errors[100];
                memset(errors, 0, sizeof(errors));
                socklen_t len = sizeof(errors);
                if(getsockopt(fds[i].fd, SOL_SOCKET, SO_ERROR, &errors, &len) < 0) {
                    printf("get scoket option failed\n");
                }
                continue;
            } else if(fds[i].revents & POLLRDHUP) {
                users[fds[i].fd] = users[fds[user_counter].fd];
                close(fds[i].fd);
                fds[i] = fds[user_counter];
                i--;
                user_counter--;
                printf("a client left\n");
            } else if(fds[i].revents & POLLIN) {
                int connfd = fds[i].fd;
                memset(users[connfd].read_buff, 0, BUFFSIZE);
                ret = recv(connfd, users[connfd].read_buff, BUFFSIZE - 1, 0);
                if(ret < 0) {
                    if(errno != EAGAIN) {
                        close(connfd);
                        users[fds[i].fd] = users[fds[user_counter].fd];
                        fds[i] = fds[user_counter];
                        i--;
                        user_counter--;
                    }
                } else if(ret == 0) {
                    
                } else {
                    for(int j = 1; j <= user_counter; j++) {
                        if(fds[j].fd == connfd) {
                            continue;
                        }

                        fds[j].events |= ~POLLIN;
                        fds[j].events |= POLLOUT;
                        users[fds[j].fd].write_buff = users[connfd].read_buff;
                    }
                }
            } else if(fds[i].revents & POLLOUT) {
                int connfd = fds[i].fd;
                if(!users[connfd].write_buff) {
                    continue;
                }
                // send the ip address from the client...
                char tmp[BUFFSIZE];
                char *ip;
                ip = inet_ntoa(users[connfd].useraddr.sin_addr);
                sprintf(tmp, "IP From: %s\n", ip);

                send(connfd, tmp, strlen(tmp), 0);
                ret = send(connfd, users[connfd].write_buff, strlen(users[connfd].write_buff), 0);

                users[connfd].write_buff = NULL;

                fds[i].events |= ~POLLOUT;
                fds[i].events |= POLLIN;
            }
        }
    }

    free(users);
    close(sockfd);

    return 0;
}

