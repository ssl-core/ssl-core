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

BENCHMARK_DIR="${PARENT_DIR}/benchmark"
TMP_GIT_REPO_DIR="/tmp/benchmark"

echo -e "\x1B[01;93mInstalling git...\n\u001b[0m"

apt install git -y

echo -e "\x1B[01;93m\nInstalling or updating Google benchmark...\n\u001b[0m"

rm -rf "${TMP_GIT_REPO_DIR}"
mkdir -p "${TMP_GIT_REPO_DIR}"

git clone "https://github.com/google/benchmark.git" -o benchmark "${TMP_GIT_REPO_DIR}"

mkdir -p "${TMP_GIT_REPO_DIR}"

rm -rf "${BENCHMARK_DIR}" # removes the directory if it exists to avoid errors
mkdir -p "${BENCHMARK_DIR}"

pushd "${TMP_GIT_REPO_DIR}" || exit 1
cmake -B build \
      -S . \
      -DBENCHMARK_ENABLE_LTO=TRUE \
      -DBENCHMARK_ENABLE_TESTING=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_INSTALL_PREFIX="${BENCHMARK_DIR}"
cmake --build build -j "$(nproc)"
cmake --install build
popd || exit 1

rm -rf "${TMP_GIT_REPO_DIR}"

chown "${CURRENT_USER}":"${CURRENT_USER}" "${BENCHMARK_DIR}" -R # changes the owner of the directory to the current user
