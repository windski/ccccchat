#ifndef THREAD_POOL_H_
#define THREAD_POOL_H_

#include <thread>
#include <condition_variable>
#include <queue>
#include <functional>
#include <memory>
#include <vector>
#include <mutex>
#include <future>

namespace chat {
namespace thread_pool {

class ThreadPool {
public:
  ThreadPool(uint32_t num);
  ~ThreadPool();
  template <typename F, typename ... Args>
  auto append(F&& f, Args&& ...)
    -> std::future<typename std::result_of<F(Args ...)>::type>;

private:
  ThreadPool(const ThreadPool &);
  ThreadPool operator=(const ThreadPool &);

  std::vector<std::thread> workers_;
  bool is_stop_;
  std::queue<std::function<void()>> workqueue_;
  std::mutex mutex_;
  std::condition_variable condi_;
};

}  // thread_pool
}  // chat


#endif

