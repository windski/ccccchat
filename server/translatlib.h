#ifndef TRANSLATLIB_H_
#define TRANSLATLIB_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define bzero(a, b) (memset(a, 0, b))

#define init_struct(a) (bzero(&(a), sizeof(a)))

#ifndef MAXLEN
#define MAXLEN 1024
#endif

#ifndef MAX_EVENTS
#define MAX_EVENTS 1024
#endif

#ifndef MAX_MSG_INFO
#define MAX_MSG_INFO 512
#endif


typedef enum {
    login,
    message,
    logout,
    unkonwn
} mes_t;

typedef enum {
    active,
    offline
} status_t;

typedef struct {
    time_t login_tm;
    time_t logout_tm;
    time_t last_active_tm;
    status_t status;
} user_info;


typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct epoll_event epoll_event;

extern int core(int sockfd);
extern int configure(const char *host, int port);
extern int run(int sockfd);
extern void setnonblock(int fileno);
extern int user_login(const char *login_mes);
extern int user_logoff(const char *logoff_mes);
extern mes_t judge_mes(const char *buff);
extern int exchange_message(const char *messages);
extern char *get_request_body(const char *msg);

#endif
