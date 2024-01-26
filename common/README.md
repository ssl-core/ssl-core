# common

This folder provides the core library for our project. It is divided into subdirectories, separated by programming language.

## C++

The C++ library is located in the `cpp` subdirectory. It can be installed inside a Docker in this repository using the following commands:

```dockerfile
COPY .cmake/ /tmp/.cmake/ # Required for the CMake configuration of the library.
COPY common/ /tmp/common/ # The library itself.

# Install the library inside the Docker in the '/usr/local/' directory.
RUN cd /tmp/common/cpp && rm -rf build && mkdir build && cd build && cmake .. -G "Ninja" -DCMAKE_CXX_COMPILER=${CXX_COMPILER} && cmake --build . && cmake --install .

# Remove the temporary files.
RUN rm -rf /tmp/.cmake/
RUN rm -rf /tmp/common/
```

Which significantly saves the compilation time of its components.

### Codemap

- [`utility`](cpp/robocin/utility/README.md): a collection of utility and helper code.