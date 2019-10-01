#include "thread_pool.h"
#include <cstdlib>
#include <cassert>

namespace chat {
namespace thread_pool {

ThreadPool::ThreadPool(uint32_t num)
  : is_stop_(false) {

  for (uint32_t i = 0; i < num; i++) {
    workers_.emplace_back(
        [this] {
          while(true) {
            std::function<void()> t;
            {
              std::unique_lock<std::mutex> lock(this->mutex_);
              this->condi_.wait(lock,
                  [this]{ return this->is_stop_ || !this->workqueue_.empty(); });

              if (this->is_stop_ && this->workqueue_.empty()) {
                return;
              }

              t = std::move(this->workqueue_.front());
              this->workqueue_.pop();
            }

            t();
          }
        }
    );
  }

}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    is_stop_ = true;
  }

  condi_.notify_all();

  for (auto &iter : workers_) {
    iter.join();
  }
}

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


#ifdef TEST
#include <iostream>

int main(void) {
  chat::thread_pool::ThreadPool pool(5);
  auto result = pool.append([](int num) { return num; }, 11);
  std::cout << result.get() << std::endl;

  return 0;
}

#endif
