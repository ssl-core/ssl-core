FROM robocin/clang-cpp23-ubuntu-latest:manual-experimental-v7

# - MARK: Install MongoDB C++ driver
# 1. Clone the MongoDB C++ driver repository
RUN git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable --depth 1
# 2. Build and install the MongoDB C++ driver
RUN cd /mongo-cxx-driver && rm -rf build && mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release -DMONGOCXX_OVERRIDE_DEFAULT_INSTALL_PREFIX=OFF -G "Ninja" .. && sudo cmake --build . --target install
# 3. Delete MongoDB C++ driver folder
RUN rm -rf /mongo-cxx-driver

RUN mkdir /app

WORKDIR /app

COPY . .

RUN cd common/cpp && rm -rf build && mkdir build && cd build && cmake .. -G "Ninja" -DCMAKE_CXX_COMPILER=clang++-17 && cmake --build . && cmake --install .

WORKDIR /app/vision-be/vision-panettone

RUN rm -rf build && mkdir build && cd build && cmake .. -G "Ninja" -DCMAKE_CXX_COMPILER=clang++-17 && cmake --build .