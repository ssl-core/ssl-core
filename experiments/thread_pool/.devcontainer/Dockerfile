FROM robocin/clang-cpp23-ubuntu-latest:manual-experimental-v7

COPY .cmake/ /tmp/.cmake/
COPY common/ /tmp/common/

RUN cd /tmp/common/cpp && rm -rf build && mkdir build && cd build && cmake .. -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ && cmake --build . && cmake --install .

RUN rm -rf /tmp/.cmake/
RUN rm -rf /tmp/common/