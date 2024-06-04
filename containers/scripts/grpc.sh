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
  PARENT_DIR="/opt"
fi

GRPC_DIR="${PARENT_DIR}"
TMP_GRPC="/tmp/grpc"

echo -e "\x1B[01;93m\nInstalling or updating grpc...\n\u001b[0m"

# reference: https://grpc.io/docs/languages/cpp/quickstart/#install-other-required-tools
apt install build-essential autoconf libtool pkg-config -y

rm -rf "${TMP_GRPC}"
mkdir -p "${TMP_GRPC}"

git clone --recurse-submodules -b "v${VERSION}" --depth 1 --shallow-submodules "https://github.com/grpc/grpc.git" -o grpc "${TMP_GRPC}"

pushd "${TMP_GRPC}" || exit 1
cmake -B build \
      -S . \
      -DgRPC_BUILD_TESTS=OFF \
      -DgRPC_INSTALL=ON \
      -DCMAKE_INSTALL_PREFIX="${GRPC_DIR}"
cmake --build build -j "$(nproc)"
cmake --install build
popd || exit 1

rm -rf "${TMP_GRPC}"
