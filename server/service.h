#ifndef SERVICE_H_
#define SERVICE_H_

#include "core.h"
#include "thread_pool.h"
#include "poll.h"

#include <functional>
#include <map>

namespace chat {
namespace io {
using thread_pool::ThreadPool;
typedef std::function<void()> callback_t;

typedef struct {
  callback_t rcallback, wcallback;
} rw_callback_t;

class Service {
 public:
  Service();
  ~Service();
  void register_readcb(int sockfd, callback_t cb);
  void register_writecb(int sockfd, callback_t cb);
  void run();
 private:
  io::Poll poll_;
  ThreadPool threads_;
  std::map<int, rw_callback_t> channel_;
};

}  // io
}  // chat

#endif
