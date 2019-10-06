#ifndef POLL_H_
#define POLL_H_

#include "core.h"
#include <poll.h>
#include <vector>

namespace chat {
namespace io {

class Poll {
 public:
  explicit Poll(int num);
  ~Poll();
  int register_fd(int sockfd, int events);
  int unregister_fd(int sockfd);
  std::vector<std::vector<int>> poll(int timeout);
 private:
  std::vector<pollfd> fds_;
  int num_;

  void init();
};

}  // io
}  // chat

#endif
