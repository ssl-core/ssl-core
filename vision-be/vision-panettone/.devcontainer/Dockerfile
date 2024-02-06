FROM robocin/clang-cpp23-ubuntu-latest:manual-experimental-v7

COPY .cmake/ /tmp/.cmake/
COPY common/ /tmp/common/
COPY . .

# - MARK: Install MongoDB C++ driver
# 1. Clone the MongoDB C++ driver repository
RUN git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable --depth 1
# 2. Build and install the MongoDB C++ driver
RUN cd /mongo-cxx-driver && rm -rf build && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DMONGOCXX_OVERRIDE_DEFAULT_INSTALL_PREFIX=OFF -G "Ninja" .. && sudo cmake --build . --target install

RUN cd /tmp/common/cpp && rm -rf build && mkdir build && cd build && cmake .. -G "Ninja" -DCMAKE_CXX_COMPILER=clang++-17 && cmake --build . && cmake --install .
RUN rm -rf /tmp/.cmake/
RUN rm -rf /tmp/common/