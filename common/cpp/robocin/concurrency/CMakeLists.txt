robocin_cpp_library(
  NAME concurrency
  HDRS thread_pool.h
       concurrent_queue.h
  SRCS thread_pool.cpp
       concurrent_queue.cpp
)

robocin_cpp_test(
  NAME thread_pool_test
  SRCS thread_pool_test.cpp
  DEPS concurrency GTest::gtest
)
