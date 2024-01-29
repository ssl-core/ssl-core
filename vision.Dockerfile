FROM robocin/clang-cpp23-ubuntu-latest:manual-experimental-v7

RUN mkdir /app

WORKDIR /app

COPY . .

RUN cd common/cpp && rm -rf build && mkdir build && cd build && cmake .. && make -j$(nproc) && make install -j$(nproc)

WORKDIR /app/vision-be/vision-panettone

RUN rm -rf build && mkdir build && cd build && cmake .. && make -j$(nproc)