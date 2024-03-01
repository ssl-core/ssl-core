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

class IThreadPool {
 public:
  explicit IThreadPool() = default;
  virtual ~IThreadPool() = default;

  IThreadPool(const IThreadPool&) = delete;
  IThreadPool& operator=(const IThreadPool&) = delete;
  IThreadPool(IThreadPool&&) = delete;
  IThreadPool& operator=(IThreadPool&&) = delete;

  virtual void restart() = 0;
  virtual void stop() = 0;

  template <typename F, typename... Args>
  auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>;
};

class ThreadPool : public IThreadPool {
 public:
  explicit ThreadPool(size_t num_threads);
  ~ThreadPool() override;

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  template <typename F, typename... Args>
  auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>;
  void restart() override;
  void stop() override;

 private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_;

  bool workAvailable();
  void workLoop();
};

/**
 * @brief Enqueues a task to be executed by a thread in the pool when
    the ThreadPool is not stopped.
 * @tparam F Type of the function to be executed.
 * @tparam Args Types of the arguments to be passed to the function.
 * @param f Function to be executed.
 * @param args Arguments to be passed to the function.
 * @return A future representing the result of the function execution.
 */
template <typename F, typename... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
  using return_type = std::invoke_result_t<F, Args...>;

  auto task
      = std::shared_ptr<std::packaged_task<return_type()>>(new std::packaged_task<return_type()>(
          std::bind_front(std::forward<F>(f), std::forward<Args>(args)...)));

  auto result = task->get_future();

  {
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
