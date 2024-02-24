# Containers

This directory contains the base dockerfiles used for development in containers, and scripts that install the necessary dependencies for these images.

> **See**: [Visual Studio Code: Developing inside a Container](https://code.visualstudio.com/docs/devcontainers/containers).

## Dockerfiles

The Dockerfiles here must be manually published to Docker Hub so that they can be reused in CIs and save time building containers. To publish manually, use the [Dockerfile: Build and Push Manually](https://github.com/robocin/ssl-core/actions/workflows/dockerfile-build-and-push.yaml) workflow. Stable images must be labeled with the `latest` tag, and development images are always labeled with the `manual-` prefix. The `context` for the workflow is the **root** of the repository; i.e, for an image located in this directory, it is equivalent to running:

```bash
  docker build -f containers/<Dockerfile> .
```

> **See**: RobôCIn's [Docker Hub](https://hub.docker.com/u/robocin) for the published images and [Development Containers Images](https://github.com/devcontainers/images/tree/main/src) for the standard base images.

### [ssl-core-cpp-base.Dockerfile](ssl-core-cpp-base.Dockerfile)

_The base image for C++ development with `ssl-core`_.

### [ssl-core-python-base.Dockerfile](ssl-core-python-base.Dockerfile)

_The base image for Python development with `ssl-core`_.

## Scripts

### [benchmark](scripts/benchmark.sh)

Install [Google Benchmark](https://github.com/google/benchmark) library.

- **Arguments:**

  1. `library_directory`: The directory where the library will be installed
       * *default*: `/usr/local`

- **Usage:**

```bash
bash benchmark.sh <library_directory>
```

### [buf](scripts/buf.sh)

Install [Buf](https://buf.build/docs/installation) CLI.

- **Arguments:**
  1. `version`: The version of Buf to install.
  2. `executable_directory`: The directory where the executable will be installed
       * *default*: `/usr/local/bin`

- **Usage:**

```bash
bash buf.sh <version> <executable_directory>
```

### [cmake](scripts/cmake.sh)

Install [CMake](https://cmake.org/) build system.

- **Arguments:**
  1. `version`: The version of CMake to install

- **Usage:**

```bash
bash cmake.sh <version>
```

### [cppzmq](scripts/cppzmq.sh)

Install [cppzmq](https://github.com/zeromq/cppzmq) library.

- **Arguments:**

  1. `version`: The version of cppzmq to install
  2. `library_directory`: The directory where the library will be installed
       * *default*: `/usr/local`

- **Usage:**

```bash
bash cppzmq.sh <version> <library_directory>
```

> **Note:** The `cppzmq` library must be installed after and in the same directory as the [`libzmq`](#libzmq) library.

### [gcc](scripts/gcc.sh)

Install [GCC](https://gcc.gnu.org/) compiler.

- **Arguments:**
  1. `version`: The version of GCC to install

- **Usage:**

```bash
bash gcc.sh <version>
```

### [googletest](scripts/googletest.sh)

Install [GoogleTest](https://github.com/google/googletest) library.

> **Note**: GoogleTest follows the Abseil Live at Head philosophy, which means that it is always in a stable state and any changes are always backwards compatible.

- **Arguments:**

  1. `version`: The version of GoogleTest to install
  2. `library_directory`: The directory where the library will be installed
       * *default*: `/usr/local`

- **Usage:**

```bash
bash googletest.sh <version> <library_directory>
```

### [libzmq](scripts/libzmq.sh)

Install [libzmq](https://github.com/zeromq/libzmq) library.

- **Arguments:**

  1. `version`: The version of libzmq to install
  2. `library_directory`: The directory where the library will be installed
       * *default*: `/usr/local`

- **Usage:**

```bash
bash libzmq.sh <version> <library_directory>
```

### [llvm](scripts/llvm.sh)

Install [LLVM](https://llvm.org/) tools: `clang`, `clang++`, `clang-format`, `clang-tidy`, `clangd`.

- **Arguments:**

  1. `version`: The version of LLVM to install

- **Usage:**

```bash
bash llvm.sh <version>
```

### [ninja](scripts/ninja.sh)

Install [Ninja](https://ninja-build.org/) build system.

- **Arguments:**

  1. `version`: The version of Ninja to install

- **Usage:**

```bash
bash ninja.sh <version>
```

### [protobuf](scripts/protobuf.sh)

Install [Protocol Buffers](https://developers.google.com/protocol-buffers) library.

- **Arguments:**

  1. `library_directory`: The directory where the library will be installed
       * *default*: `/usr/local`

- **Usage:**

```bash
bash protobuf.sh <library_directory>
```

### [protoc](scripts/protoc.sh)

Install [Protocol Buffers](https://developers.google.com/protocol-buffers) compiler.

- **Arguments:**

  1. `version`: The version of Protoc to install
  2. `directory`: The directory where the `bin` and `include` directories will be installed
       * *default*: `/usr/local`
