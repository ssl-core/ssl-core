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

MONGODB_DIR="${PARENT_DIR}/mongocxx"
TMP_MONGODB="/tmp/mongocxx"

rm -rf "${TMP_MONGODB}"
mkdir -p "${TMP_MONGODB}"

wget "https://github.com/mongodb/mongo-cxx-driver/releases/download/r${VERSION}/mongo-cxx-driver-r${VERSION}.tar.gz" -O "${TMP_MONGODB}/mongo-cxx-driver-r${VERSION}.tar.gz"
tar -xvf "${TMP_MONGODB}/mongo-cxx-driver-r${VERSION}.tar.gz" -C "${TMP_MONGODB}"
rm -rf "${TMP_MONGODB}/mongo-cxx-driver-r${VERSION}.tar.gz"

pushd "${TMP_MONGODB}/mongo-cxx-driver-r${VERSION}" || exit 1
cmake -B build \
      -S . \
      -DENABLE_TESTS=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=OFF \
      -DMONGOCXX_OVERRIDE_DEFAULT_INSTALL_PREFIX=OFF \
      -DCMAKE_INSTALL_PREFIX="${MONGODB_DIR}"
cmake --build build -j "$(nproc)"
cmake --install build
popd || exit 1

rm -rf "${TMP_MONGODB}"

chown "${CURRENT_USER}":"${CURRENT_USER}" "${MONGODB_DIR}" -R # changes the owner of the directory to the current user