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
  PARENT_DIR="/usr/local/bin"
fi

TMP_DIR=/tmp/protoc

rm -rf "${TMP_DIR}"
mkdir -p "${TMP_DIR}"

curl -sSL "https://github.com/protocolbuffers/protobuf/releases/download/v${VERSION}/protoc-${VERSION}-linux-x86_64.zip" -o "${TMP_DIR}/protoc-${VERSION}.zip"
unzip -qo "${TMP_DIR}/protoc-${VERSION}.zip" -d "${TMP_DIR}"

mv "${TMP_DIR}/bin/protoc" "${PARENT_DIR}/protoc"

chmod +x "${PARENT_DIR}/protoc"

chown "${CURRENT_USER}":"${CURRENT_USER}" "${PARENT_DIR}/protoc" # changes the owner of the directory to the current user
