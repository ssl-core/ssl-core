#ifndef ROBOCIN_UTILITY_THREAD_POOL_H
#define ROBOCIN_UTILITY_THREAD_POOL_H

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace robocin {
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
  void restart();
  void stop();

 private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_;

  void workLoop();
};

/**
 * @brief Enqueues a task to be executed by a thread in the pool.
 * @tparam F Type of the function to be executed.
 * @tparam Args Types of the arguments to be passed to the function.
 * @param f Function to be executed.
 * @param args Arguments to be passed to the function.
 * @return A future representing the result of the function execution.
 */
template <typename F, typename... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<std::result_of_t<F(Args...)>> {
  using return_type = std::result_of_t<F(Args...)>;

  auto task
      = std::shared_ptr<std::packaged_task<return_type()>>(new std::packaged_task<return_type()>(
          std::bind(std::forward<F>(f), std::forward<Args>(args)...)));

  auto result = task->get_future();

  {
    // If the ThreadPool is stopped, there is no way to enqueue any
    // task and so it throws a runtime error.
    if (stop_) {
      throw std::runtime_error("enqueue on stopped ThreadPool.");
    }
    std::unique_lock<std::mutex> lock(queue_mutex_);
    tasks_.emplace([task]() { (*task)(); });
  }

  condition_.notify_one();

  return result;
}
} // namespace robocin

#endif // ROBOCIN_UTILITY_THREAD_POOL_H
