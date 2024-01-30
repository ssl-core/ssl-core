// ThreadPool.h

#ifndef VISION_SRC_THREAD_POOL_H
#define VISION_SRC_THREAD_POOL_H

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

  ThreadPool(const ThreadPool& other) = delete;
  ThreadPool& operator=(const ThreadPool& other) = delete;
  ThreadPool(ThreadPool&& other) = delete;
  ThreadPool& operator=(ThreadPool&& other) = delete;

  template <typename F, typename... Args>
  auto enqueue(F&& f, Args&&... args) -> std::future<std::result_of_t<F(Args...)>>;

 private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_;
};

#endif // VISION_SRC_THREAD_POOL_H
