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

LIBZMQ_DIR="${PARENT_DIR}"
TMP_LIBZMQ="/tmp/libzmq"

rm -rf "${TMP_LIBZMQ}"
mkdir -p "${TMP_LIBZMQ}"

wget "https://github.com/zeromq/libzmq/releases/download/v${VERSION}/zeromq-${VERSION}.tar.gz" -O "${TMP_LIBZMQ}/zeromq-${VERSION}.tar.gz"
tar -xvf "${TMP_LIBZMQ}/zeromq-${VERSION}.tar.gz" -C "${TMP_LIBZMQ}"
rm -rf "${TMP_LIBZMQ}/zeromq-${VERSION}.tar.gz"

pushd "${TMP_LIBZMQ}/zeromq-${VERSION}" || exit 1
cmake -B build \
      -S . \
      -DZMQ_BUILD_TESTS=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="${LIBZMQ_DIR}"
cmake --build build -j "$(nproc)"
cmake --install build
popd || exit 1

rm -rf "${TMP_LIBZMQ}"
