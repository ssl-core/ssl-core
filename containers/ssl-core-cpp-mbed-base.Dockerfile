FROM mcr.microsoft.com/devcontainers/base:ubuntu

SHELL ["/bin/bash", "-c"]

ARG               GCC_VERSION='13'
ARG ARM_NONE_EABI_GCC_VERSION='13.2.rel1'
ARG              LLVM_VERSION='18'
ARG             CMAKE_VERSION='3.29.3'
ARG             NINJA_VERSION='1.11.1'
ARG               BUF_VERSION='1.28.1'
ARG           MBED_OS_VERSION='mbed-os-6.17.0'
ARG          PROTOBUF_VERSION='3.21.10'
ARG            NANOPB_VERSION='0.4.8'

ARG NANOPB_CMAKE_ARGS="-DCMAKE_C_COMPILER=arm-none-eabi-gcc -DCMAKE_C_FLAGS='--specs=nosys.specs'"

COPY containers/scripts /tmp/scripts
WORKDIR /tmp/scripts

RUN set -x && \
  apt update && apt upgrade -y && \
  apt install python3 python3-pip python-is-python3 cargo -y && \
  \
  bash   gcc.sh                        ${GCC_VERSION} && \
  bash arm-none-eabi.gcc ${ARM_NONE_EABI_GCC_VERSION} && \
  bash  llvm.sh                       ${LLVM_VERSION} && \
  bash cmake.sh                      ${CMAKE_VERSION} && \
  bash ninja.sh                      ${NINJA_VERSION} && \
  \
  bash protobuf.sh '/usr/local' && \
  bash nanopb.sh ${NANOPB_VERSION} '/usr/local' ${NANOPB_CMAKE_ARGS} && \
  \
  bash buf.sh ${BUF_VERSION} '/usr/local/bin' && \
  \
  rm -rf /tmp/scripts && \
  \
  : # last line

WORKDIR /

# Install Python modules (which are not included in requirements.txt),
# set up mbed environment and configure mbed build system
RUN set -x && \
  \
  pip install -U mbed-cli mbed-tools mbed-flash && \
  \
  wget -q https://github.com/ARMmbed/mbed-os/raw/${MBED_OS_VERSION}/requirements.txt && \
  pip install -r requirements.txt && \
  rm requirements.txt && \
  \
  mbed config -G GCC_ARM_PATH /usr/local/bin/ && \
  mbed toolchain -G -s GCC_ARM && \
  : # last line

# Display, check and save environment settings
RUN set -x -o pipefail && \
  arm-none-eabi-gcc --version | grep arm-none-eabi-gcc | tee env_settings && \
  cmake --version | grep version | tee -a env_settings && \
  python --version 2>&1 | tee -a env_settings && \
  (echo -n 'mbed-cli ' && mbed --version) | tee -a env_settings && \
  (echo -n 'mbed-greentea ' && mbedgt --version | grep ^[0-9]) | tee -a env_settings && \
  (echo -n 'mbed-host-tests ' && mbedhtrun --version) | tee -a env_settings && \
  (echo -n 'mbed-tools ' && mbed-tools --version) | tee -a env_settings && \
  : # last line
