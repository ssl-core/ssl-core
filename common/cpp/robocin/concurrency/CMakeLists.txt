robocin_cpp_library(
  NAME thread_pool
  HDRS thread_pool.h
  SRCS thread_pool.cpp
)

robocin_cpp_test(
  NAME thread_pool_test
  SRCS thread_pool_test.cpp
  DEPS thread_pool GTest::gtest
)
