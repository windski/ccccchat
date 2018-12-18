#define _GNU_SOURCE 1

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <libgen.h>
#include <strings.h>

#define BUFFSIZE 64

int main(int argc, char *args[])
{
    if(argc <= 2) {
        printf("usage: %s ip_address port number\n", basename(args[0]));
        return 1;
    }

    const char *ip = args[1];
    int port = atoi(args[2]);

    struct sockaddr_in sockaddr;
    bzero(&sockaddr, sizeof(sockaddr));

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &sockaddr.sin_addr);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    if(connect(sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr)) < 0) {
        printf("Connection failed\n");
        close(sockfd);
        return -1;
    }

    struct pollfd fds[2];

    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    fds[1].fd = sockfd;
    fds[1].events = POLLIN | POLLRDHUP;
    fds[1].revents = 0;

    char buff[BUFFSIZE];
    int pipefd[2];
    int ret = pipe(pipefd);
    assert(ret != -1);

    while(1) {
        ret = poll(fds, 2, -1);
        if(ret < 0) {
            printf("poll failure\n");
            break;
        }

        if(fds[1].revents & POLLRDHUP) {
            printf("server close the connection\n");
            break;
        } else if(fds[1].revents & POLLIN) {
            memset(buff, 0, BUFFSIZE);
            recv(fds[1].fd, buff, BUFFSIZE - 1, 0);
            printf("\n%s\n", buff);
        }

        if(fds[0].revents & POLLIN) {
            ret = splice(STDIN_FILENO, NULL, pipefd[1], NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
            ret = splice(pipefd[0], NULL, sockfd, NULL, 32768, SPLICE_F_MORE | SPLICE_F_MOVE);
        }
    }

    close(sockfd);
    return 0;
}


