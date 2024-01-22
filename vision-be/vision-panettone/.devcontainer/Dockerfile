FROM robocin/clang-cpp23-ubuntu-latest:manual-experimental-v7

COPY .cmake/ /tmp/.cmake/
COPY common/ /tmp/common/
COPY protocols/ /tmp/protocols/

RUN cd /tmp/common/cpp && rm -rf build && mkdir build && cd build && cmake .. && make -j$(nproc) && make install -j$(nproc)
RUN cd /tmp/protocols && rm -rf build && mkdir build && cd build && cmake .. && make -j$(nproc) && make install -j$(nproc)

RUN rm -rf /tmp/.cmake/
RUN rm -rf /tmp/common/
RUN rm -rf /tmp/protocols/
