#ifndef CORE_H_
#define CORE_H_

#define _GNU_SOURCE 1

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>

#include "third_party/log.h"

#define BUFFSIZE 4096

#define _LIMIT 65535

typedef struct {
    struct sockaddr_in useraddr;
    char *write_buff;
    char read_buff[BUFFSIZE];
} user_t;

typedef void (*func_callback)(void *);

#endif
