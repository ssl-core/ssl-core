FROM robocin/ssl-core-cpp-base:latest

COPY .cmake/ /tmp/.cmake/
COPY common/ /tmp/common/

RUN pushd /tmp/common/cpp && rm -rf build && \
  cmake -B build -S . -DCMAKE_CXX_COMPILER=clang++ && \
  cmake --build build && cmake --install build && \
  popd && rm -rf /tmp/.cmake/ /tmp/common/
