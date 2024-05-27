FROM robocin/ssl-core-cpp-mbed-base:latest AS devcontainer

COPY .cmake/ /tmp/.cmake/
COPY common/ /tmp/common/
COPY protocols/ /tmp/protocols/

RUN set -x && \
  pushd /tmp/common/cpp && rm -rf build && \
  cmake -B build -S . -G "Ninja" -DCMAKE_CXX_COMPILER=arm-none-eabi-g++ -DCMAKE_CXX_FLAGS='--specs=nosys.specs' -DCMAKE_C_COMPILER=arm-none-eabi-gcc -DCMAKE_C_FLAGS='--specs=nosys.specs' && \
  cmake --build build && cmake --install build && popd && \
  \
  pushd /tmp/protocols && rm -rf build && \
  cmake -B build -S . -G "Ninja" -DCMAKE_CXX_COMPILER=arm-none-eabi-g++ -DCMAKE_CXX_FLAGS='--specs=nosys.specs' -DCMAKE_C_COMPILER=arm-none-eabi-gcc -DCMAKE_C_FLAGS='--specs=nosys.specs' && \
  cmake --build build && cmake --install build && popd && \
  \
  rm -rf /tmp/.cmake/ /tmp/common/ /tmp/protocols/ && \
  : # last line
