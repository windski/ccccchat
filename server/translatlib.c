#include "translatlib.h"
#include "utils.h"
#include "rb_tree.h"

int configure(const char *host, int port)
{
    sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    if(host != NULL) {
        inet_pton(AF_INET, host, &servaddr.sin_addr);
    } else {
        servaddr.sin_addr.s_addr = INADDR_ANY;
    }
    servaddr.sin_port = htons(port);
    servaddr.sin_family = AF_INET;

    int fileno = socket(AF_INET, SOCK_STREAM, 0);

    if(bind(fileno, (sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind error\n");
        exit(-1);
    }

    if(listen(fileno, MAXLEN) < 0) {
        perror("listen error\n");
        exit(-1);
    }

    return fileno;
}


void setnonblock(int fileno)
{
    int flag;

    if((flag = fcntl(fileno, F_GETFL, 0)) < 0) {
        perror("fcntl get error");
        exit(-2);
    }

    flag |= O_NONBLOCK;

    if(fcntl(fileno, F_SETFL, flag) < 0) {
        perror("fcntl set error");
        exit(-2);
    }
}


int core(int sockfd)
{
    char buff[MAXLEN];
    bzero(&buff, sizeof(buff));

    if(read(sockfd, buff, sizeof(buff)) < 0) {
        printf("read sock error\n");
        return 1;
    }

    mes_t mess;
    mess = judge_mes(buff);

    tree_head_t user_table;
    init_tree_header(&user_table);

    // TODO: using multithread..

    switch(mess) {
    case login:
        return user_login(sockfd, &user_table, buff);
    case logout:
        return user_logoff(sockfd, &user_table, buff);
    case message:
        return exchange_message(&user_table, buff);
    default:
        printf("??\n");
    }

    return 0;
}


mes_t judge_mes(const char *buff)
{
    char *ch_l = strchr(buff, '[');

    switch(*(ch_l + 4)) {
        case 'i':
            return login;
        case 's':
            return message;
        case 'o':
            return logout;
    }
    return unkonwn;
}


int run(int sockfd)
{
    sockaddr_in cliaddr;
    socklen_t addrlen;

    addrlen = sizeof(cliaddr);

    epoll_event ev, events[MAX_EVENTS];
    int nfds, enfd, conn_sock;

    enfd = epoll_create1(0);
    if(enfd == -1) {
        perror("epoll_create1 error");
        exit(-1);
    }

    ev.events = EPOLLIN;
    ev.data.fd = sockfd;

    if(epoll_ctl(enfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
        perror("epoll_ctl error");
        exit(-1);
    }

    while(1) {
        nfds = epoll_wait(enfd, events, MAX_EVENTS, -1);
        if(nfds == -1) {
            perror("epoll_wait error");
            exit(-1);
        }

        for(int i = 0; i < nfds; i++) {
            if(events[i].data.fd == sockfd) {
                conn_sock = accept(sockfd, (sockaddr *)&cliaddr, &addrlen);

                if(conn_sock == -1) {
                    perror("accept error");
                    exit(-1);
                }

                setnonblock(conn_sock);

                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if(epoll_ctl(enfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                    perror("epoll_ctl error");
                    exit(-1);
                }
            } else {
                core(events[i].data.fd);
            }
        }
    }
    return 0;
}


char *get_request_body(const char *msg)
{
    char *tmp = strchr(msg, '\r');
    if(*(tmp + 1) == '\n') {
        return tmp + 2;
    } else {
        return NULL;
    }
}


int user_login(int sockfd, tree_head_t *user_table, const char *login_mes)
{
    user_info_t user;
    init_struct(user);

    pair_t *kv_data = NULL;
    kv_data = parse_info(login_mes);

    if(search_user(user_table, kv_data) == 0) {
        // insert_user(user_table, kv_data);
    } else {
        char buff[MAX_MSG_INFO];
        memset(buff, 0, sizeof(buff));

        sprintf(buff, "%s\r\n%s\r\n\r\n", "[ERROR]", "Can't search the target User.");
        write(sockfd, buff, sizeof(buff));
    }

    close(sockfd);

    free_kv_pair(kv_data);
    return 0;
}


int user_logoff(int sockfd, tree_head_t *user_table, const char *logoff_mes)
{
    pair_t *kv_data = NULL;
    kv_data = parse_info(logoff_mes);

    // TODO: add sothing.....

    free_kv_pair(kv_data);
    return 0;
}


int exchange_message(tree_head_t *user_table, const char *messages)
{

    return 0;
}

