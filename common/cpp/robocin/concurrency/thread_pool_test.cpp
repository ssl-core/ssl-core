#include "robocin/concurrency/thread_pool.h"

#include <gtest/gtest.h>

namespace robocin {
/**
 * @brief Enqueue a task and verify its execution.
 */
TEST(ThreadPoolTest, EnqueueAndExecuteTask) {
  ThreadPool pool(/*num_threads=*/4);

  auto future = pool.enqueue([]() { return 42; });
  int result = future.get();

  EXPECT_EQ(result, 42);
}

/**
 * @brief Enqueue multiple tasks and verify their execution.
 */
TEST(ThreadPoolTest, EnqueueMultipleTasks) {
  ThreadPool pool(4);

  auto future1 = pool.enqueue([]() { return 1; });
  auto future2 = pool.enqueue([]() { return 2; });
  auto future3 = pool.enqueue([]() { return 3; });

  int result1 = future1.get();
  int result2 = future2.get();
  int result3 = future3.get();

  EXPECT_EQ(result1, 1);
  EXPECT_EQ(result2, 2);
  EXPECT_EQ(result3, 3);
}

/**
 * @brief Enqueue a task with arguments and verify its execution.
 */
TEST(ThreadPoolTest, EnqueueTaskWithArguments) {
  ThreadPool pool(/*num_threads=*/4);

  auto future = pool.enqueue([](int a, int b) { return a + b; }, 2, 3);

  int result = future.get();

  EXPECT_EQ(result, 5);
}

/**
 * @brief Enqueue a task with void return type.
 */
TEST(ThreadPoolTest, EnqueueTaskWithVoidReturnType) {
  ThreadPool pool(/*num_threads=*/4);

  auto future = pool.enqueue([]() {});

  future.get();

  // No assertions related to result, just checking if it
  // compiles and runs without errors.
}

/**
 * @brief Enqueue a task that throws an exception also throws the exception.
 */
TEST(ThreadPoolTest, EnqueueTaskAndThrowException) {
  ThreadPool pool(/*num_threads=*/4);

  // Enqueue a task that throws an exception.
  auto future = pool.enqueue([]() { throw std::runtime_error("Test exception"); });

  // Check if getting the result from future throws the expected exception.
  EXPECT_THROW(future.get(), std::runtime_error);
}

/**
 * @brief Try to enqueue tasks after the thread pool is stopped and throws exception.
 */
TEST(ThreadPoolTest, EnqueueTasksAfterThreadPoolStopped) {
  ThreadPool pool(/*num_threads=*/4);

  // Stop the thread pool.
  pool.stop();

  // Try to enqueue a task after the thread pool is stopped
  EXPECT_THROW(pool.enqueue([]() {}), std::runtime_error);
}

/**
 * @brief Try to enqueue tasks after the thread pool is stopped and restarted works.
 */
TEST(ThreadPoolTest, EnqueueTasksAfterThreadPoolStoppedAndRestarted) {
  ThreadPool pool(/*num_threads=*/4);

  // Stop the thread pool.
  pool.stop();

  // Restart the thread pool.
  pool.restart();

  auto future = pool.enqueue([](int a, int b) { return a + b; }, 2, 3);

  int result = future.get();

  EXPECT_EQ(result, 5);
}
} // namespace robocin
