FROM robocin/ssl-core-cpp-base:latest AS devcontainer

COPY .cmake/ /tmp/.cmake/
COPY common/ /tmp/common/
COPY protocols/ /tmp/protocols/

RUN set -x && \
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
    rm -rf /tmp/.cmake/ /tmp/common/ /tmp/protocols/ && \
    : # last line

FROM devcontainer AS build

COPY . /

WORKDIR /decision-ms/decision-guardiola

RUN set -x && \
    rm -rf build bin && \
    cmake -B build -S . -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ && \
    cmake --build build && \
    : # last line

FROM scratch AS prod

COPY --from=build /decision-ms/decision-guardiola/bin/decision_main /decision_main

ENTRYPOINT [ "./decision_main" ]
