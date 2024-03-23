FROM robocin/ssl-core-cpp-base:latest

COPY .cmake/ /tmp/.cmake/
COPY common/ /tmp/common/
COPY protocols/ /tmp/protocols/

RUN \
  pushd /tmp/common/cpp && rm -rf build && \
  cmake -B build -S . -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ && \
  cmake --build build && cmake --install build && \
  popd && \
  \
  pushd /tmp/protocols && rm -rf build && \
  cmake -B build -S . -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ && \
  cmake --build build && cmake --install build && \
  popd && \
  \
  rm -rf /tmp/*
