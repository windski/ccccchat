#include "socket.h"


namespace chat {
namespace net {

Socket::Socket()
  : sockfd_(-1) {
  bzero(&sockaddr_, sizeof(sockaddr_));

  sockfd_ = socket(PF_INET, SOCK_STREAM, 0);
  assert(sockfd_ >= 0);
}

Socket::~Socket() {}

void Socket::bind(const int &port) {
  // configure the sock address
  sockaddr_.sin_family = AF_INET;
  sockaddr_.sin_port = htons(port);
  sockaddr_.sin_addr.s_addr = INADDR_ANY;

  if(::bind(sockfd_, (struct sockaddr *)&sockaddr_, sizeof(sockaddr_)) == -1) {
    log_fatal("Can't bind the port");
    exit(-1);
  }
}

int Socket::listen(int num)
{
  if(::listen(sockfd_, num) == -1) {
    log_fatal("make listen failure");
    exit(-1);
  }

  return 0;
}

int Socket::fd() const {
  return sockfd_;
}


}  // net
}  // chat

