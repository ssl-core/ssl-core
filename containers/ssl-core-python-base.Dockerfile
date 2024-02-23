FROM mcr.microsoft.com/vscode/devcontainers/python:latest

ARG PROTOC_VERSION='25.3'

COPY containers/scripts /tmp/scripts
WORKDIR /tmp/scripts

RUN apt update && apt upgrade -y && \
  bash protoc.sh ${PROTOC_VERSION} '/usr/local/bin'

RUN rm -rf /tmp/scripts
