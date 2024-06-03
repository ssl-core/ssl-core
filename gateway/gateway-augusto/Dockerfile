FROM robocin/ssl-core-cpp-base:latest as devcontainer

ARG SOURCE_DIRECTORY

COPY .cmake/ /tmp/.cmake/
COPY common/ /tmp/common/

RUN \
  pushd /tmp/common/cpp && rm -rf build && \
  cmake -B build -S . -G "Ninja" -DCMAKE_CXX_COMPILER=clang++ && \
  cmake --build build && cmake --install build && popd && \
  \
  rm -rf /tmp/.cmake/ /tmp/common/

# TODO($ISSUE_N): Keep only the necessary files for the final image.
COPY . /workspaces/

RUN \
  apt-get install nlohmann-json3-dev -y && \
  if [ -z "${SOURCE_DIRECTORY}" ]; then \
  rm -rf /workspaces/; \
  else \
  pushd "/workspaces/${SOURCE_DIRECTORY}"; \
  rm -rf build bin; \
  cmake -B build -S . -G "Ninja" -DCMAKE_CXX_COMPILER=clang++; \
  cmake --build build; \
  popd; \
  fi