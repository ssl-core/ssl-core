# common

This folder provides the core library for our project. It is divided into subdirectories, separated by programming language.

## C++

The C++ library is located in the `cpp` subdirectory. It can be installed inside a Docker in this repository using the following commands:

```dockerfile
# Required for the CMake configuration of the library.
COPY .cmake/ /tmp/.cmake/
# The library itself.
COPY common/ /tmp/common/

# Install the library inside the Docker in the '/usr/local/' directory.
RUN pushd /tmp/common/cpp && rm -rf build && cmake -B build -S . -DCMAKE_CXX_COMPILER=${CXX_COMPILER} && cmake --build build && cmake --install build && popd && rm -rf /tmp/.cmake/ /tmp/common/
```

Which significantly saves the compilation time of its components.

### Codemap

- [`utility`](cpp/robocin/utility/README.md): a collection of utility and helper code.
