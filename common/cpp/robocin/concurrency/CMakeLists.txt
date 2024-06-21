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
