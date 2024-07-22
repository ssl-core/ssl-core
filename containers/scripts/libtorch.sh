#!/bin/bash

function is_root {
  [ "${EUID:-$(id -u)}" -eq 0 ];
}

if ! is_root; then
  echo -e "\x1B[31m[ERROR] This script requires root privileges."
  exit 1
fi

VERSION="${1}"
# see compute platform at: https://pytorch.org
COMPUTE_PLATFORM="${2}"
PARENT_DIR="${3}"
CURRENT_USER=$(who | awk 'NR==1{print $1}')

if [ -z "${VERSION}" ]; then
  echo -e "\x1B[31m[ERROR] No version specified."
  exit 1
fi

if [ -z "${COMPUTE_PLATFORM}" ]; then
  echo -e "\x1B[31m[ERROR] No compute platform specified."
  exit 1
fi

if [ -z "${PARENT_DIR}" ]; then
  PARENT_DIR="/usr/local"
fi

LIBTORCH_DIR="${PARENT_DIR}"
TMP_LIBTORCH="/tmp/libtorch"

rm -rf "${TMP_LIBTORCH}"
mkdir -p "${TMP_LIBTORCH}"

wget "https://download.pytorch.org/libtorch/${COMPUTE_PLATFORM}/libtorch-cxx11-abi-static-with-deps-${VERSION}%2B${COMPUTE_PLATFORM}.zip" -O "${TMP_LIBTORCH}/libtorch.zip"
unzip -o "${TMP_LIBTORCH}/libtorch.zip" -d "${TMP_LIBTORCH}"

rsync -a --include="*/***" --exclude="*" "${TMP_LIBTORCH}/libtorch/" "${LIBTORCH_DIR}/"

rm -rf "${TMP_LIBTORCH}"
