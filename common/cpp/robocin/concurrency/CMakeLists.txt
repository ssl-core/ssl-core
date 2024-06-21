robocin_cpp_library(
  NAME concurrency
  HDRS thread_pool.h blocking_deque.h
  SRCS thread_pool.cpp blocking_deque.cpp
)

robocin_cpp_test(
  NAME thread_pool_test
  SRCS thread_pool_test.cpp
  DEPS concurrency GTest::gtest
)

robocin_cpp_library(
  NAME concurrent_queue
  HDRS concurrent_queue.h
  SRCS concurrent_queue.cpp
)
