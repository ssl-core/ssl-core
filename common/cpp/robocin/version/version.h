#ifndef ROBOCIN_VERSION_VERSION_H
#define ROBOCIN_VERSION_VERSION_H

#if __has_include(<zmq.h>)
#define __robocin_lib_zmq 202405L
#endif

#if __has_include(<zmq.hpp>)
#define __robocin_lib_cppzmq 202405L
#endif

#if __has_include(<gtest/gtest_prod.h>)
#define __robocin_lib_googletest 202405L
#endif

#endif // ROBOCIN_VERSION_VERSION_H
