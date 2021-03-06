#include "core.h"
#include <poll.h>


void recv_file(int sockfd, uint64_t size)
{
    char buff[BUFFSIZE];
    bzero(buff, sizeof(buff));

    // just store it in a regular file.
    int filefd = open("test.data", O_CREAT | O_WRONLY, 0666);
    if(filefd < 0) {
        perror("open()");
        close(filefd);
        return ;
    }

    ssize_t recv_size;
    while(size > 0) {
        recv_size = recv(sockfd, buff, 1024, 0);
        if(recv_size < 0) {
            perror("recv");
            break;
        }

        write(filefd, buff, recv_size);
        size -= recv_size;
    }

    close(filefd);
}


int main(int argc, const char *args[])
{
    if(argc <= 2) {
        fprintf(stderr, "usage: %s <ip> <port>", args[0]);
        exit(-1);
    }

    const char *ip = args[1];
    int port = atoi(args[2]);

    struct pollfd fds[2];
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &servaddr.sin_addr);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd > 0);

    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect: connection failure");
        close(sockfd);
        return 1;
    }

    // fill fd..
    // standard input
    fds[0].fd = STDIN_FILENO;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    // for remote server.
    fds[1].fd = sockfd;
    fds[1].events = POLLIN | POLLRDHUP;
    fds[1].revents = 0;


    while(1) {
        int ret = poll(fds, 2, -1);
        if(ret < 0) {
            perror("poll failure");
            break;
        }

        if(fds[1].revents & POLLRDHUP) {
            // server may dropped.
            fprintf(stderr, "server close the conntion\n");
            break;
        } else if(fds[1].revents & POLLIN) {
            // receive the data.
            char size_buf[1024];
            char buff[BUFFSIZE];
            if(recv(sockfd, size_buf, 1024, 0) < 0) {
                perror("recv: Something goes error.");
                break;
            }

            uint64_t _size = atoll(size_buf);

            if(_size == -1) {
                // command get wrong.
                recv(sockfd, buff, BUFFSIZE, 0);
                printf("%s\n", buff);
                continue;
            }
            printf("it receive %lu bytes data from remote peer.\n", _size);

            recv_file(sockfd, _size);
            break;
        }

        if(fds[0].revents & POLLIN) {
            char buff_in[1024];
            char buff[BUFFSIZE];
            if(fgets(buff_in, sizeof(buff_in), stdin) == NULL) {
                printf("exit.\n");
                break;
            }
            // remove the `\n' from stdin
            buff_in[strlen(buff_in) - 1] = '\0';

            int len = strlen(buff_in);
            printf("len: %d\n", len);

            bzero(buff, sizeof(buff));
            sprintf(buff, "%d", len);
            if(send(sockfd, buff, 1024, 0) < 0) {
                perror("send failure");
                break;
            }
            bzero(buff, sizeof(buff));

            if(send(sockfd, buff_in, len, 0) < 0) {
                perror("send failure");
                break;
            }

        }
    }

    close(sockfd);

    return 0;
}

