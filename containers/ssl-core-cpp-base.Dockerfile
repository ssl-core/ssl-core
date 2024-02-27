FROM mcr.microsoft.com/vscode/devcontainers/base:latest

ARG    GCC_VERSION='13'
ARG   LLVM_VERSION='18'
ARG  CMAKE_VERSION='3.29.0-rc2'
ARG  NINJA_VERSION='1.11.1'
ARG    BUF_VERSION='1.28.1'
ARG LIBZMQ_VERSION='4.3.5'
ARG CPPZMQ_VERSION='4.10.0'

COPY containers/scripts /tmp/scripts
WORKDIR /tmp/scripts

RUN apt update && apt upgrade -y && \
  \
  bash   gcc.sh   ${GCC_VERSION} && \
  bash  llvm.sh  ${LLVM_VERSION} && \
  bash cmake.sh ${CMAKE_VERSION} && \
  bash ninja.sh ${NINJA_VERSION} && \
  \
  bash googletest.sh '/usr/local' && \
  bash  benchmark.sh '/usr/local' && \
  \
  bash protobuf.sh '/usr/local' && \
  \
  bash libzmq.sh ${LIBZMQ_VERSION} '/usr/local' && \
  bash cppzmq.sh ${CPPZMQ_VERSION} '/usr/local' && \
  \
  bash buf.sh ${BUF_VERSION} '/usr/local/bin' && \
  \
  rm -rf /tmp/scripts
