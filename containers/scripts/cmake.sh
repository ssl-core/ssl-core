#!/bin/bash

function is_root {
  [ "${EUID:-$(id -u)}" -eq 0 ];
}

if ! is_root; then
  echo -e "\x1B[31m[ERROR] This script requires root privileges."
  exit 1
fi

VERSION="${1}"

if [ -z "${VERSION}" ]; then
  echo -e "\x1B[31m[ERROR] No version specified."
  exit 1
fi

echo -e "\x1B[01;93mInstalling or updating cmake...\n\u001b[0m"

TMP_DIR="/tmp/cmake"

wget "https://github.com/Kitware/CMake/releases/download/v${VERSION}/cmake-${VERSION}.tar.gz" -O "${TMP_DIR}/cmake.tar.gz"
tar -xvf "${TMP_DIR}/cmake.tar.gz" -C "${TMP_DIR}"

pushd "${TMP_DIR}/cmake-${VERSION}" || exit 1
./bootstrap
make -j$(nproc)
make install -j$(nproc)
popd || exit 1

rm -rf "${TMP_DIR}"
