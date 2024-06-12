FROM mcr.microsoft.com/devcontainers/base:noble

SHELL ["/bin/bash", "-c"]

ARG      GCC_VERSION='14'
ARG     LLVM_VERSION='18'
ARG    CMAKE_VERSION='3.29.3'
ARG    NINJA_VERSION='1.11.1'
ARG PROTOBUF_VERSION='3.21.10'
ARG   LIBZMQ_VERSION='4.3.5'
ARG   CPPZMQ_VERSION='4.10.0'
ARG      BUF_VERSION='1.28.1'

COPY containers/scripts /tmp/scripts
WORKDIR /tmp/scripts

RUN apt update && apt upgrade -y && \
  \
  bash   gcc.sh   ${GCC_VERSION} && \
  bash  llvm.sh  ${LLVM_VERSION} && \
  bash cmake.sh ${CMAKE_VERSION} && \
  bash ninja.sh ${NINJA_VERSION} && \
  \
  bash       absl.sh '/usr/local' && \
  bash googletest.sh '/usr/local' && \
  bash  benchmark.sh '/usr/local' && \
  \
  bash protobuf.sh ${PROTOBUF_VERSION} '/usr/local' && \
  \
  bash libzmq.sh ${LIBZMQ_VERSION} '/usr/local' && \
  bash cppzmq.sh ${CPPZMQ_VERSION} '/usr/local' && \
  \
  bash buf.sh ${BUF_VERSION} '/usr/local/bin' && \
  \
  rm -rf /tmp/scripts
