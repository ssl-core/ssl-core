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

GOOGLETEST_DIR="${PARENT_DIR}"
TMP_GIT_REPO_DIR="/tmp/googletest"

echo -e "\x1B[01;93m\nInstalling or updating googletest...\n\u001b[0m"

rm -rf "${TMP_GIT_REPO_DIR}"
mkdir -p "${TMP_GIT_REPO_DIR}"

git clone --recurse-submodules "https://github.com/google/googletest.git" -o googletest "${TMP_GIT_REPO_DIR}"

mkdir -p "${TMP_GIT_REPO_DIR}"

pushd "${TMP_GIT_REPO_DIR}" || exit 1
cmake -B build \
      -S . \
      -DCMAKE_INSTALL_PREFIX="${GOOGLETEST_DIR}"
cmake --build build -j "$(nproc)"
cmake --install build
popd || exit 1

rm -rf "${TMP_GIT_REPO_DIR}"
