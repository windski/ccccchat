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

template <typename F, typename ... Args>
auto ThreadPool::append(F &&f, Args&&... args)
  -> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared< std::packaged_task<return_type()> >(
    std::bind(std::forward<F>(f), std::forward<Args>(args)...)
  );

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(mutex_);

    // don't allow enqueueing after stopping the pool
    if(is_stop_)
      throw std::runtime_error("enqueue on stopped ThreadPool");

    workers_.emplace_back([task](){ (*task)(); });
  }

  condi_.notify_one();
  return res;
}

}  // thread_pool
}  // chat


#endif

