#ifndef ROBOCIN_VERSION_VERSION_H
#define ROBOCIN_VERSION_VERSION_H

#include <version>

#if defined(_GLIBCXX_HAS_GTHREADS)
#define __robocin_lib_std_concurrency 202405L
#endif

#if __has_include(<zmq.h>)
#define __robocin_lib_zmq 202405L
#endif

#if __has_include(<zmq.hpp>)
#define __robocin_lib_cppzmq 202405L
#endif

#if __has_include(<gtest/gtest_prod.h>)
#define __robocin_lib_googletest 202405L
#endif

#if __has_include(<arpa/inet.h>)
#define __robocin_lib_arpa_inet 202405L
#endif

#endif // ROBOCIN_VERSION_VERSION_H
