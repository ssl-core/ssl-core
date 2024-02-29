#include "robocin/concurrency/thread_pool.h"

namespace robocin {

/**
 * @brief Constructs a ThreadPool object with the specified number of threads.
 * @param num_threads The number of threads in the thread pool.
 */
ThreadPool::ThreadPool(size_t num_threads) : stop_(false) {
  workers_.reserve(num_threads);

  for (size_t i = 0; i < num_threads; ++i) {
    workers_.emplace_back(&ThreadPool::workLoop, this);
  }
}

/**
 * @brief Restarts the thread pool by initializing worker threads.
 *
 * This method restarts the thread pool by initializing the worker threads, allowing
 * the thread pool to be used for enqueuing tasks again after it has been stopped.
 *
 * @note If the thread pool is already running, calling this method has no effect.
 *       Use the stop method to stop the thread pool before calling start again.
 */
void ThreadPool::restart() {
  stop_ = false;
  for (auto& worker : workers_) {
    worker = std::thread([this] { workLoop(); });
  }
}

/**
 * @brief Stops the thread pool and joins all worker threads.
 *
 * This method stops the thread pool and joins all worker threads. After calling this
 * method, the thread pool can no longer be used to enqueue tasks.
 */
void ThreadPool::stop() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }

  condition_.notify_all();

  // Join all worker threads
  for (std::thread& worker : workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

/**
 * @brief Destroys the ThreadPool object.
 *
 * Calls the `stop` function to stop the thread pool.
 */
ThreadPool::~ThreadPool() { this->stop(); }

/**
 * @brief Indicates whether the worker can execute tasks.
 *
 * It checks if the thread pool is stopped and if there are
 * tasks available to be executed.
 */
bool ThreadPool::workAvailable() { return this->stop_ || !this->tasks_.empty(); }

/**
 * @brief Worker thread loop function.
 *
 * Waits for tasks to be added to the task queue and executes them.
 */
void ThreadPool::workLoop() {
  while (true) {
    std::function<void()> task;

    {
      std::unique_lock<std::mutex> lock(this->queue_mutex_);

      this->condition_.wait(lock, [this] { return workAvailable(); });

      if (this->stop_ && this->tasks_.empty()) {
        return;
      }

      task = std::move(this->tasks_.front());
      this->tasks_.pop();
    }

    task();
  }
}

} // namespace robocin
