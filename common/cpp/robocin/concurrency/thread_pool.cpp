#include "robocin/concurrency/thread_pool.h"

#include <functional>

namespace robocin {

ThreadPool::ThreadPool(size_t num_threads) : is_stopped_{false} {
  workers_.reserve(num_threads);

  for (size_t i = 0; i < num_threads; ++i) {
    workers_.emplace_back(&ThreadPool::workLoop, this);
  }
}

void ThreadPool::stop() {
  {
    std::unique_lock lock(works_mutex_);
    is_stopped_ = true;
  }

  cv_.notify_all();
}

ThreadPool::~ThreadPool() { stop(); }

void ThreadPool::workLoop() {
  std::move_only_function<void()> work;

  while (true) {
    {
      std::unique_lock lock(works_mutex_);
      cv_.wait(lock, [this]() { return !works_.empty() || is_stopped_; });

      if (works_.empty() && is_stopped_) {
        return;
      }

      work = std::move(works_.front());
      works_.pop();
    }

    work();
  }
}

} // namespace robocin
