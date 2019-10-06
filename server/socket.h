#ifndef SOCKET_H_
#define SOCKET_H_

#include "core.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace chat {
namespace net {

class Socket {
 public:
  Socket();
  ~Socket();
  int listen(int num);
  void bind(const int &port);
  int fd() const;

 private:
  int sockfd_;
  struct sockaddr_in sockaddr_;
};

}  // net
}  // chat

#endif
