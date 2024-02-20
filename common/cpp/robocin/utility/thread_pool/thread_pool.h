// ThreadPool.h

#ifndef ROBOCIN_UTILITY_THREAD_POOL_H
#define ROBOCIN_UTILITY_THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  explicit ThreadPool(size_t num_threads);
  ~ThreadPool();

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  template <typename F, typename... Args>
  auto enqueue(F&& f, Args&&... args) -> std::future<std::result_of_t<F(Args...)>>;

 private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;
  std::mutex queueMutex_;
  std::condition_variable condition_;
  bool stop_;
};

template <typename F, typename... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<std::result_of_t<F(Args...)>> {
  using return_type = std::result_of_t<F(Args...)>;

  auto task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  auto result = task->get_future();

  {
    std::unique_lock<std::mutex> lock(queueMutex_);

    if (stop_) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }

    tasks_.emplace([task]() { (*task)(); });
  }

  condition_.notify_one();

  return result;
}

#endif // ROBOCIN_UTILITY_THREAD_POOL_H