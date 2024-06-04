# version

A collection of version-specific headers.

## Table of Contents

- [version](#version)
  - [Table of Contents](#table-of-contents)
  - [`version`](#version-1)

## [`version`](version.h)

The [version](version.h) header provides macros and utilities to handle the ssl-core common/cpp library versioning. This allows for easy management and identification of the C++ standard and extenal libraries being used in the project, ensuring compatibility and leveraging the latest features available.

The `version.h` file consists of the following key components:
- `__robocin_lib_std_concurrency`: Macro that checks for the availability of the GNU thread support library;
- `__robocin_lib_zmq`: Macro that verifies the inclusion of the `<zmq.h>` ZeroMQ C library header;
- `__robocin_lib_cppzmq`: Macro that verifies the inclusion of the `<zmq.hpp>` ZeroMQ C++ binding header;
- `__robocin_lib_googletest`: Macro that checks for the inclusion of the `<gtest/gtest_prod.h>` Google Test production header;
- `__robocin_lib_arpa_inet`: Macro that verifies the inclusion of the `arpa/inet.h` header for network operations.
