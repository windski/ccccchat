#include "socket.h"

// default to use ipv4 family
int create_socket(const char *ip, const int port)
{
    int sockfd = -1;
    struct sockaddr_in sockaddr;
    bzero(&sockaddr, sizeof(sockaddr));

    // configure the sock address
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    if(ip) {
        inet_pton(AF_INET, ip, &sockaddr.sin_addr);
    } else {
        sockaddr.sin_addr.s_addr = INADDR_ANY;
    }

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    if(bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1) {
        perror("Can't bind the port");
        exit(-1);
    }

    return sockfd;
}


int make_listen(int fd, int num)
{
    if(listen(fd, num) == -1) {
        perror("make listen failure");
        exit(-1);
    }

    return 0;
}

