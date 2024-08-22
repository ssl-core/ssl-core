FROM mcr.microsoft.com/devcontainers/go:1.22

RUN set -x && \
    apt update && apt upgrade -y && \
    \
    apt install libsodium-dev libczmq-dev protobuf-compiler -y && \
    \
    : # last line

RUN set -x && \
    go install google.golang.org/protobuf/cmd/protoc-gen-go@v1.28 && \
    go install google.golang.org/grpc/cmd/protoc-gen-go-grpc@v1.2 && \
    \
    : # last line
