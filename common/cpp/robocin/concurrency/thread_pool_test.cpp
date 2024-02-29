#include "robocin/concurrency/thread_pool.h"

#include <cstddef>
#include <future>
#include <gtest/gtest.h>
#include <stdexcept>

namespace robocin {

/*
 * @class ThreadPoolSpy
 * @brief A spy class for testing ThreadPool.
 *
 * This class is designed to be used in unit tests to spy on the behavior of a ThreadPool.
 * It inherits from the IThreadPool interface to provide a mock implementation with additional
 * functionality for testing purposes.
 */
class ThreadPoolSpy : public IThreadPool {
 public:
  explicit ThreadPoolSpy(size_t num_threads) : workers_size_(num_threads){};

  void stop() override { stop_ = true; }
  void restart() override { stop_ = false; }

  template <typename F, typename... Args>
  auto enqueue(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
    using return_type = std::invoke_result_t<F, Args...>;
    auto task
        = std::shared_ptr<std::packaged_task<return_type()>>(new std::packaged_task<return_type()>(
            std::bind_front(std::forward<F>(f), std::forward<Args>(args)...)));
    auto result = task->get_future();

    {
      if (isStopped()) {
        throw std::runtime_error("enqueue on stopped ThreadPool.");
      }
      tasks_.emplace([task]() { (*task)(); });
    }

    return result;
  }

  void runEnqueuedTasks() {
    size_t available_workers = workers_size_;
    while (!tasks_.empty() && ((available_workers--) > 0)) {
      std::function<void()> task;
      task = std::move(this->tasks_.front());
      this->tasks_.pop();
      task();
    }
  }

  [[nodiscard]] size_t pendentTasks() const { return tasks_.size(); }

  [[nodiscard]] bool hasTasks() const { return !tasks_.empty(); }

  [[nodiscard]] bool isStopped() const { return stop_; }

  [[nodiscard]] bool isRunning() const { return !stop_; }

 private:
  std::queue<std::function<void()>> tasks_;
  bool stop_{};
  size_t workers_size_;
};

TEST(ThreadPoolTest, WhenStopThreadPoolThenThreadPoolIsStopped) {
  ThreadPoolSpy pool(/*num_threads=*/4);

  pool.stop();

  EXPECT_TRUE(pool.isStopped());
}

TEST(ThreadPoolTest, GivenStoppedThreadPoolWhenEnqueueTaskThenThrowsRuntimeError) {
  ThreadPoolSpy pool(/*num_threads=*/4);

  pool.stop();

  EXPECT_THROW(pool.enqueue([]() {}), std::runtime_error);
}

TEST(ThreadPoolTest, WhenRestartAStoppedThreadPoolThenThreadPoolIsRestarted) {
  ThreadPoolSpy pool(/*num_threads=*/4);

  pool.stop();
  EXPECT_TRUE(pool.isStopped());

  pool.restart();
  EXPECT_TRUE(pool.isRunning());
}

TEST(ThreadPoolTest, GivenRestartedThreadPoolWhenEnqueueTaskThenReturnsTheTaskResult) {
  ThreadPoolSpy pool(/*num_threads=*/4);

  pool.stop();
  pool.restart();

  auto future = pool.enqueue([](int a, int b) { return a + b; }, 2, 3);
  EXPECT_EQ(pool.pendentTasks(), 1);

  pool.runEnqueuedTasks();

  EXPECT_EQ(future.get(), 5);
}

TEST(ThreadPoolTest, WhenEnqueueTaskThenReturnsTheTaskResult) {
  ThreadPoolSpy pool(/*num_threads=*/4);

  auto future = pool.enqueue([]() { return 1; });
  EXPECT_EQ(pool.pendentTasks(), 1);

  pool.runEnqueuedTasks();

  EXPECT_EQ(future.get(), 1);
}

TEST(ThreadPoolTest, WhenEnqueueMultipleTasksThenReturnsTheTaskResults) {
  ThreadPoolSpy pool(/*num_threads=*/4);

  auto future1 = pool.enqueue([]() { return 1; });
  auto future2 = pool.enqueue([]() { return 2; });
  auto future3 = pool.enqueue([]() { return 3; });
  EXPECT_EQ(pool.pendentTasks(), 3);

  pool.runEnqueuedTasks();

  EXPECT_EQ(future1.get(), 1);
  EXPECT_EQ(future2.get(), 2);
  EXPECT_EQ(future3.get(), 3);
}

TEST(ThreadPoolTest, WhenEnqueueTaskWithArgumentsThenReturnsTheTaskResultForProvidedArguments) {
  ThreadPoolSpy pool(/*num_threads=*/4);

  auto future = pool.enqueue([](int a, int b) { return a + b; }, 2, 3);
  EXPECT_EQ(pool.pendentTasks(), 1);

  pool.runEnqueuedTasks();

  EXPECT_EQ(future.get(), 5);
}

TEST(ThreadPoolTest, WhenEnqueueTaskThatThrowsExceptionThenThrowsSameException) {
  ThreadPoolSpy pool(/*num_threads=*/4);

  auto future = pool.enqueue([]() { throw std::logic_error("Test exception"); });
  EXPECT_EQ(pool.pendentTasks(), 1);

  pool.runEnqueuedTasks();

  EXPECT_THROW(future.get(), std::logic_error);
}

TEST(ThreadPoolTest, WhenEnqueueMoreTasksThanWorkersThenTasksCallingAreLimitedByWorkersSize) {
  ThreadPoolSpy pool(/*num_threads=*/4);

  auto future1 = pool.enqueue([]() { return 1; });
  auto future2 = pool.enqueue([]() { return 2; });
  auto future3 = pool.enqueue([]() { return 3; });
  auto future4 = pool.enqueue([]() { return 4; });
  auto future5 = pool.enqueue([]() { return 5; });
  EXPECT_EQ(pool.pendentTasks(), 5);

  pool.runEnqueuedTasks();

  EXPECT_TRUE(pool.hasTasks());
}
} // namespace robocin
