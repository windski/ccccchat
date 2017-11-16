#ifndef COMM_STRUCT_H_
#define COMM_STRUCT_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/epoll.h>

#ifndef MAXLEN
#define MAXLEN 1024
#endif

#ifndef MAX_EVENTS
#define MAX_EVENTS 1024
#endif

#ifndef MAX_MSG_INFO
#define MAX_MSG_INFO 512
#endif

#ifndef MAX_USER_L
#define MAX_USER_L 32
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
    char to[MAX_USER_L];
    char *mes_buff;
} mes_buf_t;

typedef struct {
    time_t login_tm;
    time_t logout_tm;
    time_t last_active_tm;
    status_t status;
    char user_name[MAX_USER_L];
    mes_buf_t mes_buff;
    void *friends;            // point to a friends of table
    char user_passwd[MAX_USER_L];             // unsafe...screw it
} user_info_t;


typedef enum { RED, BLACK } rb_color_t;

typedef struct node {
    user_info_t *data;
    struct node *parent;
    struct node *left;
    struct node *right;
    rb_color_t color;
}rb_node_t;


typedef struct {
    int length;
    rb_node_t *root;
}tree_head_t;


#endif
