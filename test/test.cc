#include <cstdlib>
#include <strings.h>
#include <vector>
#include <cstring>
#include <poll.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cassert>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

#include <pthread.h>

static const int BUFFSIZE = 64;

using namespace std;

void *test_connect(void *arg)
{

    const char *ip = "45.63.56.234";
    int port = 8000;

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
        return NULL;
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
    return NULL;
}

int main(void)
{
    pid_t pid;

    for(int i = 0; i < 10; i++) {

        if((pid = fork()) == 0) {
            continue;
        } else {
            vector<pthread_t> tid;

            for(int i = 0; i < 200; i++) {
                pthread_t tmp;
                pthread_create(&tmp, NULL, test_connect, NULL);

                tid.emplace_back(tmp);
            }

            for(auto i : tid) {
                pthread_join(i, NULL);
            }
        } 
    }

    return 0;
    
}

