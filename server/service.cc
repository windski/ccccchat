#include "service.h"

#include <cassert>

namespace chat {
namespace io {

Service::Service() 
  : poll_(1024),
  threads_(20) {}

Service::~Service() {}

void Service::register_readcb(int sockfd, callback_t cb) {
  assert(sockfd > 0);

  auto iter = channel_.find(sockfd);
  if (iter == channel_.end()) {
    rw_callback_t tmpobj;
    tmpobj.rcallback = cb;
    channel_.emplace(sockfd, tmpobj);

    poll_.register_fd(sockfd, POLLIN | POLLERR);
  }

  channel_.at(sockfd).rcallback = cb;
}

void Service::register_writecb(int sockfd, callback_t cb) {
  assert(sockfd > 0);

  auto iter = channel_.find(sockfd);
  if (iter == channel_.end()) {
    rw_callback_t tmpobj;
    tmpobj.wcallback = cb;
    channel_.emplace(sockfd, tmpobj);

    poll_.register_fd(sockfd, POLLOUT);
  }

  channel_.at(sockfd).wcallback = cb;
}

void Service::run() {
  while(1) {
    auto matrix = poll_.poll(-1);
    // read events
    std::vector<int> list = matrix[0];
    for (auto i : list) {
      threads_.append(channel_.at(i).rcallback);
    }

    // write events
    list = matrix[1];
    for (auto i : list) {
      threads_.append(channel_.at(i).wcallback);
    }
  }
}


}  // io
}  // chat

