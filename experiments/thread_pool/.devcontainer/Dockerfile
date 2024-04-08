FROM robocin/ssl-core-cpp-base:latest

COPY .cmake/ /tmp/.cmake/
COPY common/ /tmp/common/

RUN cd /tmp/common/cpp && rm -rf build && mkdir build && cd build && cmake .. -DCMAKE_CXX_COMPILER=clang++ && cmake --build . && cmake --install .

RUN rm -rf /tmp/.cmake/
RUN rm -rf /tmp/common/
