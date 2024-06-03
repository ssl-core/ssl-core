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

ABSL_DIR="${PARENT_DIR}"
TMP_GIT_REPO_DIR="/tmp/absl"

rm -rf "${TMP_GIT_REPO_DIR}"
mkdir -p "${TMP_GIT_REPO_DIR}"

git clone --recurse-submodules "https://github.com/abseil/abseil-cpp.git" -o absl "${TMP_GIT_REPO_DIR}"

mkdir -p "${TMP_GIT_REPO_DIR}"

pushd "${TMP_GIT_REPO_DIR}" || exit 1
cmake -B build \
      -S . \
      -DCMAKE_BUILD_TYPE=Release \
      -DABSL_BUILD_TESTING=OFF \
      -DCMAKE_INSTALL_PREFIX="${ABSL_DIR}"
cmake --build build -j "$(nproc)"
cmake --install build
popd || exit 1

rm -rf "${TMP_GIT_REPO_DIR}"
