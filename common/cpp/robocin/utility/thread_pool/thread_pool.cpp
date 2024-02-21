#include "robocin/utility/thread_pool/thread_pool.h"

ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
  for (size_t i = 0; i < num_threads; ++i) {
    workers_.emplace_back([this] {
      while (true) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(this->queueMutex_);
          this->condition_.wait(lock, [this] { return this->stop_ || !this->tasks_.empty(); });

          if (this->stop_ && this->tasks_.empty()) {
            return;
          }

          task = std::move(this->tasks_.front());
          this->tasks_.pop();
        }

        task();
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(queueMutex_);
    stop_ = true;
  }

  condition_.notify_all();

  for (std::thread& worker : workers_) {
    worker.join();
  }
}