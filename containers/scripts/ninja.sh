#!/bin/bash

function is_root {
  [ "${EUID:-$(id -u)}" -eq 0 ];
}

if ! is_root; then
  echo -e "\x1B[31m[ERROR] This script requires root privileges."
  exit 1
fi

VERSION="${1}"
CURRENT_USER=$(who | awk 'NR==1{print $1}')

if [ -z "${VERSION}" ]; then
  echo -e "\x1B[31m[ERROR] No version specified."
  exit 1
fi

DEST_DIR="/usr/bin"
TMP_DIR="/tmp/ninja"

echo -e "\x1B[01;93mInstalling or updating ninja...\n\u001b[0m"

rm -rf "${TMP_DIR}"
mkdir -p "${TMP_DIR}"

git clone --branch "v${VERSION}" --depth 1 --shallow-submodules "https://github.com/ninja-build/ninja.git" -o ninja "${TMP_DIR}"

pushd "${TMP_DIR}" || exit 1
cmake -B build
cmake --build build -j "$(nproc)"
mv -f "build/ninja" "${DEST_DIR}/ninja"
popd || exit 1

rm -rf "${TMP_DIR}"

chmod +x "${DEST_DIR}/ninja"

# changes the owner of the directory to the current user
chown "${CURRENT_USER}":"${CURRENT_USER}" "${DEST_DIR}/ninja"
