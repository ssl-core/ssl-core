#!/bin/bash

function is_root {
  [ "${EUID:-$(id -u)}" -eq 0 ];
}

if ! is_root; then
  echo -e "\x1B[31m[ERROR] This script requires root privileges."
  exit 1
fi

VERSION="3.21.10" # TODO(#73): Allow the user to specify the version.
PARENT_DIR="${1}"
CURRENT_USER=$(who | awk 'NR==1{print $1}')

if [ -z "${VERSION}" ]; then
  echo -e "\x1B[31m[ERROR] No version specified."
  exit 1
fi

if [ -z "${PARENT_DIR}" ]; then
  PARENT_DIR="/usr/local"
fi

PROTOBUF_DIR="${PARENT_DIR}"
TMP_PROTOBUF="/tmp/protobuf"

echo -e "\x1B[01;93m\nInstalling or updating protobuf...\n\u001b[0m"

rm -rf "${TMP_PROTOBUF}"
mkdir -p "${TMP_PROTOBUF}"

wget "https://github.com/protocolbuffers/protobuf/releases/download/v21.10/protobuf-cpp-${VERSION}.tar.gz" -O "${TMP_PROTOBUF}/protobuf-${VERSION}.tar.gz"
tar -xvf "${TMP_PROTOBUF}/protobuf-${VERSION}.tar.gz" -C "${TMP_PROTOBUF}"
rm -rf "${TMP_PROTOBUF}/protobuf-${VERSION}.tar.gz"

pushd "${TMP_PROTOBUF}/protobuf-${VERSION}" || exit 1
cmake -B build \
      -S . \
      -DCMAKE_BUILD_TYPE=Release \
      -Dprotobuf_ABSL_PROVIDER=package \
      -Dprotobuf_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX="${PROTOBUF_DIR}"
cmake --build build -j "$(nproc)"
cmake --install build
popd || exit 1

rm -rf "${TMP_PROTOBUF}"
