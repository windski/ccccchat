#include "core.h"
#include <poll.h>

int main(int argc, const char *args[])
{
    if(argc < 2) {
        fprintf(stderr, "usage: %s -a <ip> -p <port>", args[0]);
        exit(-1);
    }

    int m_stop = 0;
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

    char buff[BUFFSIZE];
    bzero(&buff, sizeof(buff));

    while(!m_stop) {
        int ret = poll(fds, 2, -1);
        if(ret < 0) {
            perror("poll failure");
            break;
        }

        if(fds[1].revents & POLLRDHUP) {
            // server may restart
            fprintf(stderr, "server close the conntion\n");
            m_stop = 1;
        } else if(fds[1].revents & POLLIN) {
            // TODO: fin.
        }

        if(fds[0].revents & POLLIN) {
            char buff_in[1024];
            if(fgets(buff_in, sizeof(buff_in), stdin) == NULL) {
                fprintf(stderr, "fgets: no input read.\n");
                continue;
            }

            int len = strlen(buff_in);
            printf("len: %d\n", len);

            sprintf(buff, "%d", len);
            if(send(sockfd, buff, 4, 0) < 0) {
                perror("send failure");
                m_stop = 1;
            }
            bzero(buff, sizeof(buff));

            if(send(sockfd, buff_in, len, 0) < 0) {
                perror("send failure");
                m_stop = 1;
            }

        }
    }

    close(sockfd);

    return 0;
}

