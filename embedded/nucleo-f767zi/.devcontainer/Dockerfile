FROM robocin/ssl-core-cpp-base:latest

ARG MBED_OS_VER=mbed-os-6.17.0

RUN apt install python3 python3-pip python-is-python3 -y

# ------------------------------------------------------------------------------
# Install Python modules (which are not included in requirements.txt)
RUN set -x \
  && pip install -U \
  mbed-cli \
  mbed-tools \
  && : # last line

# Set up mbed environment
WORKDIR /tmp/
RUN set -x \
  && wget -q https://github.com/ARMmbed/mbed-os/raw/${MBED_OS_VER}/requirements.txt \
  && pip install -r requirements.txt  \
  && rm requirements.txt \
  && : # last line

# ------------------------------------------------------------------------------
# Install arm-none-eabi-gcc
WORKDIR /tmp/
RUN set -x \
  && [ "$(uname -m)" = "aarch64" ] && \
  TARBALL="arm-gnu-toolchain-13.2.rel1-aarch64-arm-none-eabi.tar.xz" || \
  TARBALL="arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz" \
  && wget -q https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/${TARBALL} \
  && tar -xf ${TARBALL} \
  && rsync -av --remove-source-files $(find . -maxdepth 1 -type d -regex '.*arm-none-eabi.*')/ /usr/local/ \
  && rm ${TARBALL} \
  && : # last line

# ------------------------------------------------------------------------------
# Configure mbed build system
RUN set -x \
  && mbed config -G GCC_ARM_PATH /usr/local/bin/ \
  && mbed toolchain -G -s GCC_ARM \
  && : # last line

# ------------------------------------------------------------------------------
# Display, check and save environment settings
# NOTE: using bash instead of Ubuntu default dash due to unsupport for pipefail
# Pipefail is crucial here, if the tools didn't install properly, docker build should not pass because of piping 
RUN /bin/bash -c \
  "set -x -o pipefail \
  && arm-none-eabi-gcc --version | grep arm-none-eabi-gcc | tee env_settings \
  && cmake --version | grep version | tee -a env_settings \
  && python3 --version 2>&1 | tee -a env_settings \
  && (echo -n 'mbed-cli ' && mbed --version) | tee -a env_settings \
  && (echo -n 'mbed-greentea ' && mbedgt --version | grep ^[0-9]) | tee -a env_settings \
  && (echo -n 'mbed-host-tests ' && mbedhtrun --version) | tee -a env_settings \
  && (echo -n 'mbed-tools ' && mbed-tools --version) | tee -a env_settings \
  && : # LAST LINE"
