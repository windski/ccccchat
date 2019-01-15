#ifndef SOCKET_H_
#define SOCKET_H_

#include "core.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// just create the socket...useless....
// Do not using it in productivity...maybe..
extern int create_socket(const char *ip, const int port);

extern void make_listen(int fd, int num);

#endif
