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
CMAKE_ADDITIONAL_ARGS="${3}"
CURRENT_USER=$(who | awk 'NR==1{print $1}')

CMAKE_ARGS=()
CMAKE_ARGS+=("-DCMAKE_BUILD_TYPE=Release")
CMAKE_ARGS+=("-DCMAKE_INSTALL_PREFIX=${NANOPB_DIR}")
for arg in ${CMAKE_ADDITIONAL_ARGS[@]}; do
    CMAKE_ARGS+=("$arg")
done

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

pip install --upgrade google-api-python-client # required.

rm -rf "${TMP_NANOPB}"
mkdir -p "${TMP_NANOPB}"

wget "https://github.com/nanopb/nanopb/archive/refs/tags/${VERSION}.tar.gz" -O "${TMP_NANOPB}/nanopb-${VERSION}.tar.gz"
tar -xvf "${TMP_NANOPB}/nanopb-${VERSION}.tar.gz" -C "${TMP_NANOPB}"
rm -rf "${TMP_NANOPB}/nanopb-${VERSION}.tar.gz"

pushd "${TMP_NANOPB}/nanopb-${VERSION}" || exit 1
cmake -B build -S . ${CMAKE_ARGS[@]}
cmake --build build -j "$(nproc)"
cmake --install build
popd || exit 1

rm -rf "${TMP_NANOPB}"

# changes the owner of the directory to the current user
chown "${CURRENT_USER}":"${CURRENT_USER}" "${NANOPB_DIR}" -R
