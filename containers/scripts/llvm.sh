#!/bin/bash

function is_root {
  [ "${EUID:-$(id -u)}" -eq 0 ];
}

if ! is_root; then
  echo -e "\x1B[31m[ERROR] This script requires root privileges."
  exit 1
fi

function latest-version {
  local NAME="${1}"
  local VERSION_REGEX
  local AVAILABLE_VERSIONS

  VERSION_REGEX="[0-9]+(\.[0-9]+)*"
  AVAILABLE_VERSIONS=$(apt-cache search --names-only "^${NAME}-${VERSION_REGEX}$" | grep -Po "${VERSION_REGEX}" | xargs -n1 | sort -Vr)

  echo "${AVAILABLE_VERSIONS}" | head -1
}

function add-alternative {
  local NAME="${1}"
  local VERSION="${2}"
  local FILEPATH

  FILEPATH="/usr/bin/${NAME}"

  if [ -f "${FILEPATH}-${VERSION}" ]; then
    update-alternatives --install "${FILEPATH}" "${NAME}" "${FILEPATH}-${VERSION}" "$(echo "${VERSION}" | grep -Po "^[0-9]+")"
  else
    echo -e "\x1B[31m[ERROR] ${FILEPATH}-${VERSION} does not exist."
    exit 1
  fi
}

function set-alternative {
  local NAME="${1}"
  local VERSION="${2}"
  local FILEPATH

  FILEPATH="/usr/bin/${NAME}"

  update-alternatives --set "${NAME}" "${FILEPATH}-${VERSION}"
}

VERSION="${1}"

if [ -z "${VERSION}" ]; then
  echo -e "\x1B[31m[ERROR] No version specified."
  exit 1
fi

declare -a SCRIPT_ARGS

if [ "${VERSION}" != "latest" ]; then
  SCRIPT_ARGS+=("${VERSION}")
fi

SCRIPT_ARGS+=("all")

echo -e "\x1B[01;93mInstalling or updating llvm tools (version: ${VERSION})...\n\u001b[0m"

apt install lsb-release wget software-properties-common gnupg -y

bash -c "$(wget -qO - https://apt.llvm.org/llvm.sh)" '' "${SCRIPT_ARGS[@]}"

echo -e "\u001b[35m\n--\n\u001b[0m"

for TOOL in clang clang++ clang-format clang-tidy clangd; do
  TOOL_VERSION="${VERSION}"

  if [ "${TOOL_VERSION}" = "latest" ]; then
    TOOL_VERSION=$(latest-version "${TOOL}")
  fi

  if [ -z "${TOOL_VERSION}" ]; then
    echo -e "\x1B[31m[ERROR] Could not find version '${TOOL_VERSION}' of ${TOOL}.\x1B[0m"
    exit 1
  fi

  add-alternative "${TOOL}" "${TOOL_VERSION}"
  set-alternative "${TOOL}" "${TOOL_VERSION}"

  echo -e "\x1B[01;93mNow, the updated version of ${TOOL} is: \u001b[0m${TOOL_VERSION}\n\u001b[0m"
done
