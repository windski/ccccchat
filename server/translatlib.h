#ifndef TRANSLATLIB_H_
#define TRANSLATLIB_H_

#include "comm_struct.h"

#define bzero(a, b) (memset(a, 0, b))

#define init_struct(a) (bzero(&(a), sizeof(a)))


typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct epoll_event epoll_event;

extern int core(int sockfd);
extern int configure(const char *host, int port);
extern int run(int sockfd);
extern void setnonblock(int fileno);
extern int user_login(tree_head_t *user_table, const char *login_mes);
extern int user_logoff(tree_head_t *user_table, const char *logoff_mes);
extern mes_t judge_mes(const char *buff);
extern int exchange_message(tree_head_t *user_table, const char *messages);
extern char *get_request_body(const char *msg);

#endif
