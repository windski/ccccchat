#ifndef SOCKET_H_
#define SOCKET_H_

#include "core.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

extern int create_socket(const char *ip, const int port);

extern int make_listen(int fd, int num);

#endif
