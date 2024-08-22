#ifndef ROBOCIN_CONCURRENCY_THREAD_POOL_H
#define ROBOCIN_CONCURRENCY_THREAD_POOL_H

#include "robocin/version/version.h"

#if defined(__robocin_lib_std_concurrency) and __robocin_lib_std_concurrency >= 202405L

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
  /**
   * @brief Constructs a ThreadPool object with the specified number of threads.
   * @param num_threads The number of threads in the thread pool.
   */
  explicit ThreadPool(size_t num_threads);

  /**
   * @brief Destroys the ThreadPool object.
   *
   * Calls the `stop` function to stop the thread pool.
   */
  ~ThreadPool();

  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  /**
   * @brief Enqueues a task to be executed by a thread in the pool when
      the ThreadPool is not stopped.
   * @tparam F Type of the function to be executed.
   * @tparam Args Types of the arguments to be passed to the function.
   * @param f Function to be executed.
   * @param args Arguments to be passed to the function.
   * @return A future representing the result of the function execution.
   */
  template <class F, class... Args>
  std::future<std::invoke_result_t<F, Args...>> enqueue(F&& f, Args&&... args);

  /**
   * @brief Stops the thread pool and joins all worker threads.
   *
   * This method stops the thread pool and joins all worker threads. After calling this
   * method, the thread pool can no longer be used to enqueue works.
   */
  void stop();

 private:
  /**
   * @brief Worker thread loop function.
   *
   * Waits for works to be added to the works queue and executes them.
   */
  void workLoop();

  std::mutex works_mutex_;
  std::condition_variable cv_;
  std::queue<std::move_only_function<void()>> works_;
  std::vector<std::jthread> workers_;

  bool is_stopped_;
};

template <class F, class... Args>
std::future<std::invoke_result_t<F, Args...>> ThreadPool::enqueue(F&& f, Args&&... args) {
  using return_type = std::invoke_result_t<F, Args...>;

  std::packaged_task<return_type()> task{
      std::bind_front(std::forward<F>(f), std::forward<Args>(args)...),
  };

  auto future_result = task.get_future();

  {
    std::unique_lock lock(works_mutex_);

    if (is_stopped_) {
      throw std::runtime_error("ThreadPool: enqueue on stopped thread pool.");
    }

    works_.emplace([work_fn = std::move(task)]() mutable { work_fn(); });
  }

  cv_.notify_one();

  return future_result;
}

} // namespace robocin

#endif

#endif // ROBOCIN_CONCURRENCY_THREAD_POOL_H
