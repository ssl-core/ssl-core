FROM robocin/ssl-core-cpp-mbed-base:latest

RUN pip install mbed-flasher

COPY containers/scripts /tmp/scripts

WORKDIR /tmp/scripts
RUN set -x && apt update && apt upgrade -y && \
  \
  bash protobuf.sh '/usr/local' && \
  bash nanopb.sh '0.4.8' '/usr/local' "-DCMAKE_C_COMPILER=arm-none-eabi-gcc -DCMAKE_C_FLAGS='--specs=nosys.specs'" && \
  \
  rm -rf /tmp/scripts && \
  : # last line

COPY .cmake/ /tmp/.cmake/
COPY common/ /tmp/common/
COPY protocols/ /tmp/protocols/

RUN set -x && \
  pushd /tmp/common/cpp && rm -rf build && \
  cmake -B build -S . -G "Ninja" -DCMAKE_C_COMPILER=arm-none-eabi-gcc -DCMAKE_C_FLAGS='--specs=nosys.specs' -DROBOCIN_EMBEDDED_CROSS_COMPILING=TRUE && \
  cmake --build build && cmake --install build && popd && \
  \
  pushd /tmp/protocols && rm -rf build && \
  cmake -B build -S . -G "Ninja" -DCMAKE_C_COMPILER=arm-none-eabi-gcc -DCMAKE_C_FLAGS='--specs=nosys.specs' -DROBOCIN_EMBEDDED_CROSS_COMPILING=TRUE && \
  cmake --build build && cmake --install build && popd && \
  \
  rm -rf /tmp/.cmake/ /tmp/common/ /tmp/protocols/ && \
  : # last line
