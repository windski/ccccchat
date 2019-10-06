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
