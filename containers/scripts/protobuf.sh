#!/bin/bash

function is_root {
  [ "${EUID:-$(id -u)}" -eq 0 ];
}

if ! is_root; then
  echo -e "\x1B[31m[ERROR] This script requires root privileges."
  exit 1
fi

PARENT_DIR="${1}"
CURRENT_USER=$(who | awk 'NR==1{print $1}')

if [ -z "${PARENT_DIR}" ]; then
  PARENT_DIR="/usr/local"
fi

PROTOBUF_DIR="${PARENT_DIR}/protobuf"
TMP_GIT_REPO_DIR="/tmp/protobuf"

echo -e "\x1B[01;93mInstalling git...\n\u001b[0m"

apt install git -y

echo -e "\x1B[01;93m\nInstalling or updating protobuf...\n\u001b[0m"

rm -rf "${TMP_GIT_REPO_DIR}"
mkdir -p "${TMP_GIT_REPO_DIR}"

git clone --recurse-submodules https://github.com/protocolbuffers/protobuf.git -o protobuf "${TMP_GIT_REPO_DIR}"

mkdir -p "${TMP_GIT_REPO_DIR}"

rm -rf "${PROTOBUF_DIR}" # removes the directory if it exists to avoid errors
mkdir -p "${PROTOBUF_DIR}"

pushd "${TMP_GIT_REPO_DIR}" || exit 1
cmake -B build \
      -S . \
      -DCMAKE_BUILD_TYPE=Release \
      -Dprotobuf_BUILD_TESTS=OFF \
      -DCMAKE_INSTALL_PREFIX="${PROTOBUF_DIR}"
cmake --build build -j "$(nproc)"
cmake --install build
popd || exit 1

rm -rf "${TMP_GIT_REPO_DIR}"

chown "${CURRENT_USER}":"${CURRENT_USER}" "${PROTOBUF_DIR}" -R # changes the owner of the directory to the current user
