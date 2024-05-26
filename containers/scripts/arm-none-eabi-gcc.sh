#!/bin/bash

function is_root {
  [ "${EUID:-$(id -u)}" -eq 0 ];
}

if ! is_root; then
  echo -e "\x1B[31m[ERROR] This script requires root privileges."
  exit 1
fi

VERSION="${1}"
PARENT_DIR="${2}"
CURRENT_USER=$(who | awk 'NR==1{print $1}')

if [ -z "${VERSION}" ]; then
  echo -e "\x1B[31m[ERROR] No version specified."
  exit 1
fi

if [ -z "${PARENT_DIR}" ]; then
  PARENT_DIR="/usr/local"
fi

ARM_NONE_EABI_GCC_DIR="${PARENT_DIR}"
TMP_ARM_NONE_EABI_GCC="/tmp/arm-gnu-toolchain"

if [ "$(uname -m)" = "aarch64" ]; then
  TARBALL="arm-gnu-toolchain-${VERSION}-aarch64-arm-none-eabi.tar.xz"
else
  TARBALL="arm-gnu-toolchain-${VERSION}-x86_64-arm-none-eabi.tar.xz"
fi

rm -rf "${TMP_ARM_NONE_EABI_GCC}"
mkdir -p "${TMP_ARM_NONE_EABI_GCC}"

wget "https://developer.arm.com/-/media/Files/downloads/gnu/${VERSION}/binrel/${TARBALL}" -O "${TMP_ARM_NONE_EABI_GCC}/${TARBALL}"
tar -xf "${TMP_ARM_NONE_EABI_GCC}/${TARBALL}" -C "${TMP_ARM_NONE_EABI_GCC}"

pushd "${TMP_ARM_NONE_EABI_GCC}" || exit 1
rsync -av --remove-source-files $(find . -maxdepth 1 -type d -regex '.*arm-none-eabi.*')/ "${ARM_NONE_EABI_GCC_DIR}"
popd || exit 1

rm -rf "${TMP_ARM_NONE_EABI_GCC}"
