FROM mcr.microsoft.com/devcontainers/base:ubuntu

SHELL ["/bin/bash", "-c"]

ARG     GCC_VERSION='13'
ARG    LLVM_VERSION='18'
ARG   CMAKE_VERSION='3.29.3'
ARG   NINJA_VERSION='1.11.1'
ARG     BUF_VERSION='1.28.1'
ARG MBED_OS_VERSION='mbed-os-6.17.0'
ARG  NANOPB_VERSION='0.4.8'

RUN set -x && \
  apt update && apt upgrade -y && \
  apt install python3 python3-pip python-is-python3 cargo -y && \
  : # last line

COPY containers/scripts /tmp/scripts
WORKDIR /tmp/scripts

RUN set -x && \
  \
  bash   gcc.sh   ${GCC_VERSION} && \
  bash  llvm.sh  ${LLVM_VERSION} && \
  bash cmake.sh ${CMAKE_VERSION} && \
  bash ninja.sh ${NINJA_VERSION} && \
  \
  bash protobuf.sh '/usr/local' && \
  \
  bash buf.sh ${BUF_VERSION} '/usr/local/bin' && \
  \
  : # last line

# Install Python modules (which are not included in requirements.txt)
RUN set -x && \
  pip install -U mbed-cli mbed-tools && \
  : # last line

WORKDIR /tmp/

# Set up mbed environment
RUN set -x && \
  wget -q https://github.com/ARMmbed/mbed-os/raw/${MBED_OS_VERSION}/requirements.txt && \
  pip install -r requirements.txt && \
  rm requirements.txt && \
  : # last line

# Install arm-none-eabi-gcc
RUN set -x && \
  [ "$(uname -m)" = "aarch64" ] && \
  TARBALL="arm-gnu-toolchain-13.2.rel1-aarch64-arm-none-eabi.tar.xz" || \
  TARBALL="arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz" && \
  wget -q https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/${TARBALL} && \
  tar -xf ${TARBALL} && \
  rsync -av --remove-source-files $(find . -maxdepth 1 -type d -regex '.*arm-none-eabi.*')/ /usr/local/ && \
  rm ${TARBALL} && \
  : # last line

# Install nanopb
WORKDIR /tmp/scripts

RUN set -x && \
  bash nanopb.sh ${NANOPB_VERSION} '/usr/local' "-DCMAKE_C_COMPILER=arm-none-eabi-gcc -DCMAKE_C_FLAGS='--specs=nosys.specs'" && \
  \
  rm -rf /tmp/scripts && \
  \
  : # last line

# Configure mbed build system
RUN set -x && \
  mbed config -G GCC_ARM_PATH /usr/local/bin/ && \
  mbed toolchain -G -s GCC_ARM && \
  : # last line

WORKDIR /

# Display, check and save environment settings
RUN set -x -o pipefail && \
  arm-none-eabi-gcc --version | grep arm-none-eabi-gcc | tee env_settings && \
  cmake --version | grep version | tee -a env_settings && \
  python --version 2>&1 | tee -a env_settings && \
  (echo -n 'mbed-cli ' && mbed --version) | tee -a env_settings && \
  (echo -n 'mbed-greentea ' && mbedgt --version | grep ^[0-9]) | tee -a env_settings && \
  (echo -n 'mbed-host-tests ' && mbedhtrun --version) | tee -a env_settings && \
  (echo -n 'mbed-tools ' && mbed-tools --version) | tee -a env_settings && \
  : # LAST LINE
