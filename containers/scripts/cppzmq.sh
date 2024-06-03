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

CPPZMQ_DIR="${PARENT_DIR}"
TMP_CPPZMQ="/tmp/cppzmq"

rm -rf "${TMP_CPPZMQ}"
mkdir -p "${TMP_CPPZMQ}"

wget "https://github.com/zeromq/cppzmq/archive/refs/tags/v${VERSION}.tar.gz" -O "${TMP_CPPZMQ}/cppzmq-${VERSION}.tar.gz"
tar -xvf "${TMP_CPPZMQ}/cppzmq-${VERSION}.tar.gz" -C "${TMP_CPPZMQ}"
rm -rf "${TMP_CPPZMQ}/cppzmq-${VERSION}.tar.gz"

pushd "${TMP_CPPZMQ}/cppzmq-${VERSION}" || exit 1
cmake -B build \
      -S . \
      -DCPPZMQ_BUILD_TESTS=OFF \
      -DCMAKE_PREFIX_PATH="${PARENT_DIR}/libzmq" \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="${CPPZMQ_DIR}"
cmake --build build -j "$(nproc)"
cmake --install build
popd || exit 1

rm -rf "${TMP_CPPZMQ}"
