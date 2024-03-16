#include "robocin/concurrency/thread_pool.h"

#include <future>
#include <gtest/gtest.h>
#include <stdexcept>

namespace robocin {

static constexpr int kNumThreads = 4;

TEST(ThreadPoolTest, GivenStoppedThreadPoolWhenEnqueueTaskThenThrowsRuntimeError) {
  ThreadPool thread_pool(kNumThreads);

  thread_pool.stop();

  EXPECT_THROW(thread_pool.enqueue([]() {}), std::runtime_error);
}

TEST(ThreadPoolTest, WhenEnqueueTaskThenReturnsTheTaskResult) {
  ThreadPool thread_pool(kNumThreads);

  auto future = thread_pool.enqueue([]() { return 1; });

  EXPECT_EQ(future.get(), 1);
}

TEST(ThreadPoolTest, WhenEnqueueMultipleTasksThenReturnsTheTaskResults) {
  ThreadPool thread_pool(kNumThreads);

  auto future1 = thread_pool.enqueue([]() { return 1; });
  auto future2 = thread_pool.enqueue([]() { return 2; });
  auto future3 = thread_pool.enqueue([]() { return 3; });

  EXPECT_EQ(future1.get(), 1);
  EXPECT_EQ(future2.get(), 2);
  EXPECT_EQ(future3.get(), 3);
}

TEST(ThreadPoolTest, WhenEnqueueTaskWithArgumentsThenReturnsTheTaskResultForProvidedArguments) {
  ThreadPool thread_pool(kNumThreads);

  auto future = thread_pool.enqueue([](int a, int b) { return a + b; }, 2, 3);

  EXPECT_EQ(future.get(), 5);
}

TEST(ThreadPoolTest, WhenEnqueueTaskThatThrowsExceptionThenThrowsSameException) {
  ThreadPool thread_pool(kNumThreads);

  auto future = thread_pool.enqueue([]() { throw std::logic_error("something went wrong."); });

  EXPECT_THROW(future.get(), std::logic_error);
}

TEST(ThreadPoolTest, WhenEnqueueMoreTasksThanWorkersThenReturnsTheTaskResults) {
  ThreadPool thread_pool(kNumThreads);

  auto future1 = thread_pool.enqueue([]() { return 1; });
  auto future2 = thread_pool.enqueue([]() { return 2; });
  auto future3 = thread_pool.enqueue([]() { return 3; });
  auto future4 = thread_pool.enqueue([]() { return 4; });
  auto future5 = thread_pool.enqueue([]() { return 5; });

  EXPECT_EQ(future1.get(), 1);
  EXPECT_EQ(future2.get(), 2);
  EXPECT_EQ(future3.get(), 3);
  EXPECT_EQ(future4.get(), 4);
  EXPECT_EQ(future5.get(), 5);
}

} // namespace robocin
