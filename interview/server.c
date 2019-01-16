#include "core.h"
#include "socket.h"
#include <sys/epoll.h>
#include <dirent.h>

void setnonblocking(int fd)
{
    int flags;

    if((flags = fcntl(fd, F_GETFL)) < 0) {
        perror("fcntl: get the flags failure");
        exit(-3);
    }

    flags |= O_NONBLOCK;

    if((fcntl(fd, F_SETFL)) < 0) {
        perror("fcntl: set the flags failure");
        exit(-4);
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


void removefd(int epollfd, int fd)
{
    assert(epollfd > 0);
    assert(fd > 0);

    // There is a bug in Linux Kernel before 2.6.9, more details check it in man pages.
    // I don't care cuz....
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
}


char *get_check_filename(const char *buff)
{
    assert(buff != NULL);

    // get the name
    char *name = strstr(buff, "get") + 3;
    if(name == NULL) {
        return NULL;
    }

    // skip all spaces.
    while(*(++name) == ' ' && *name);

    //check it out if exist.
    DIR *direct = opendir(".");
    struct dirent *dirinfo;
    while((dirinfo = readdir(direct)) != NULL) {
        if(strcmp(dirinfo->d_name, name) == 0) {
            closedir(direct);
            return name;
        }
    }

    closedir(direct);
    return NULL;
}


uint64_t get_filesize(const char *name)
{
    uint64_t filesize = -1;
    struct stat status;
    if(stat(name, &status) < 0) {
        fprintf(stderr, "get file status failure\n");
        return filesize;
    } else {
        filesize = status.st_size;
    }

    return filesize;
}


void trans_data(int sockfd, const char *name)
{
    char buff[BUFFSIZE];
    uint64_t filesize = get_filesize(name);
    sprintf(buff, "%lu", filesize);
    send(sockfd, buff, 4, 0);

    int filefd = open(name, O_RDONLY);
    if(sendfile(sockfd, filefd, NULL, filesize) < 0) {
        perror("sendfile: send file failure");
    }

    close(filefd);
}


int main(int argc, const char *args[])
{
    if(argc < 2) {
        fprintf(stderr, "usage: %s <port>\n", args[0]);
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
                if(count == 0) {
                    removefd(epollfd, events[i].data.fd);
                    close(events[i].data.fd);
                    continue;
                } else if(count < 0) {
                    perror("recv: failure");
                    continue;
                }
                printf("count: %s\n", buff);

                count = atoi(buff);
                if(count == 0) {
                    continue;
                }
                bzero(buff, sizeof(buff));

                // receive a command.
                count = recv(events[i].data.fd, buff, count, 0);

                printf("buff: %s\n", buff);

                char *name = get_check_filename(buff);
                if(name == NULL) {
                    // get the wrong file.
                    send(events[i].data.fd, "-1 ", 4, 0);
                    // send the tips.
                    send(events[i].data.fd, "file is not exist.", 19, 0);
                    continue;
                }

                trans_data(events[i].data.fd, name);
                // Just close the connection when the file were sent.23333
                removefd(epollfd, events[i].data.fd);
                close(events[i].data.fd);
            }

        }
    }

    return 0;
}

