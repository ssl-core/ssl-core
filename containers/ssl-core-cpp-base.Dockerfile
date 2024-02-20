FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu

ARG    GCC_VERSION='13'
ARG   LLVM_VERSION='18'
ARG  CMAKE_VERSION='3.28.1'
ARG  NINJA_VERSION='1.11.1'
ARG    BUF_VERSION='1.28.1'
ARG LIBZMQ_VERSION='4.3.5'
ARG CPPZMQ_VERSION='4.10.0'

RUN apt update && apt upgrade -y

COPY scripts /tmp/scripts
WORKDIR /tmp/scripts

RUN bash   gcc.sh   $GCC_VERSION
RUN bash  llvm.sh  $LLVM_VERSION
RUN bash cmake.sh $CMAKE_VERSION
RUN bash ninja.sh $NINJA_VERSION

RUN bash googletest.sh '/usr/local'
RUN bash  benchmark.sh '/usr/local'
RUN bash   protobuf.sh '/usr/local'

RUN bash buf.sh $BUF_VERSION '/usr/local/bin'

RUN bash libzmq.sh $LIBZMQ_VERSION '/usr/local'
RUN bash cppzmq.sh $CPPZMQ_VERSION '/usr/local'

RUN rm -rf /tmp/scripts
