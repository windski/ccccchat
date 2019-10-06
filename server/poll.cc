#include "poll.h"
#include <cassert>

namespace chat {
namespace io {

Poll::Poll(int num)
  : num_(num) {
  init();
}

Poll::~Poll() {}

void Poll::init() {
  pollfd poll;
  poll.fd = -1;
  poll.events = 0;
  poll.revents = 0;
  
  for (int i = 0; i < num_; i++) {
    fds_.push_back(poll);
  }
}

int Poll::register_fd(int sockfd, int events) {
  assert(sockfd > 0);

  for (int i = 0; i < num_; i++) {
    if (fds_[i].fd == -1) {
      fds_[i].fd = sockfd;
      fds_[i].events = events;
      fds_[i].revents = 0;

      return 0;
    }
  }

  log_fatal("have no empty slot to registe.");
  return -1;
}

int Poll::unregister_fd(int sockfd) {
  assert(sockfd > 0);

  for (int i = 0; i < num_; i++) {
    if (fds_[i].fd == sockfd) {
      fds_[i].fd = -1;
      fds_[i].events = 0;
      fds_[i].revents = 0;

      return 0;
    }
  }

  log_fatal("should not doing here.");
  assert(0);
}

std::vector<std::vector<int>> Poll::poll(int timeout) {
  int ret = ::poll(fds_.data(), num_ + 1, timeout);

  std::vector<std::vector<int>> result;
  if (ret < 0) {
    log_fatal("poll failure");
    return result;
  }

  std::vector<int> read;
  std::vector<int> write;
  for (int i = 0; i < num_ + 1; i++) {
    if (fds_[i].revents & POLLIN) {
      read.push_back(fds_[i].fd);
      ret--;
    } else if (fds_[i].revents & POLLOUT) {
      write.push_back(fds_[i].fd);
      ret--;
    }

    log_info("has redundant pollfd");
    if (ret <= 0) {
      break;
    }
  }

  result.push_back(read);
  result.push_back(write);

  return result;
}

}  // io
}  // chat

