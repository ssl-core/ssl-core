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

NANOPB_DIR="${PARENT_DIR}"
TMP_NANOPB="/tmp/nanopb"

echo -e "\x1B[01;93m\nInstalling or updating nanopb...\n\u001b[0m"

rm -rf "${TMP_NANOPB}"
mkdir -p "${TMP_NANOPB}"

wget "https://github.com/nanopb/nanopb/archive/refs/tags/${VERSION}.tar.gz" -O "${TMP_NANOPB}/nanopb-${VERSION}.tar.gz"
tar -xvf "${TMP_NANOPB}/nanopb-${VERSION}.tar.gz" -C "${TMP_NANOPB}"
rm -rf "${TMP_NANOPB}/nanopb-${VERSION}.tar.gz"

pushd "${TMP_NANOPB}/nanopb-${VERSION}" || exit 1
cmake -B build \
      -S . \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="${NANOPB_DIR}"
cmake --build build -j "$(nproc)"
cmake --install build
popd || exit 1

rm -rf "${TMP_NANOPB}"

# changes the owner of the directory to the current user
chown "${CURRENT_USER}":"${CURRENT_USER}" "${NANOPB_DIR}" -R
