FROM robocin/ssl-core-cpp-mongo:latest

ARG SOURCE_DIRECTORY

# TODO($ISSUE_N): Keep only the necessary files for the final image.
COPY . /workspaces/service/

RUN \
  if [ -z "${SOURCE_DIRECTORY}" ]; then \
    rm -rf /workspaces/service/; \
  else \
    cd "/workspaces/service/${SOURCE_DIRECTORY}"; \
    cmake -B build -S . -G "Ninja" -DCMAKE_CXX_COMPILER=clang++; \
    cmake --build build; \
  fi
