#!/bin/bash

function is_root {
  [ "${EUID:-$(id -u)}" -eq 0 ];
}

if ! is_root; then
  echo -e "\x1B[31m[ERROR] This script requires root privileges."
  exit 1
fi

function add-alternative {
  update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-"$1" "$(echo "$1" | grep -Po "^[0-9]+")"
  update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-"$1" "$(echo "$1" | grep -Po "^[0-9]+")"
}

function set-alternative {
 update-alternatives --set gcc /usr/bin/gcc-"$1"
 update-alternatives --set g++ /usr/bin/g++-"$1"
}

VERSION_REGEX="[0-9]+(\.[0-9]+)*"
VERSION="${1}"

if [ -z "${VERSION}" ]; then
  VERSION="latest"
fi

echo -e "\x1B[01;93mInstalling or updating required packages...\n\u001b[0m"

apt install software-properties-common -y
add-apt-repository ppa:ubuntu-toolchain-r/test -y
apt install build-essential -y

echo -e "\u001b[35m\n--\n\u001b[0m"

if [ -z "$(which gcc)" ]; then
  echo -e "\x1B[01;93mNo gcc/g++ compiler found."
  echo -e "\x1B[01;93mInstalling default gcc/g++ version...\n\u001b[0m"

  apt install gcc g++ -y

  echo -e "\u001b[35m\n--\n\u001b[0m"
fi

echo -e "\x1B[01;93mThe current version of gcc/g++ compiler is:\n"
echo -e "\u001b[0m$(gcc --version)"
echo -e "\u001b[35m\n--\n\u001b[0m"

CURRENT_VERSION="$(gcc -dumpversion)"
# adds current version as alternative as well.
add-alternative "${CURRENT_VERSION}"

AVAILABLE_VERSIONS=$(apt-cache search --names-only "^g\+\+\-${VERSION_REGEX}$" | grep -Po "${VERSION_REGEX}" | xargs -n1 | sort -Vr)
LATEST_VERSION=$(echo "${AVAILABLE_VERSIONS}" | head -1)

echo -e "\x1B[01;93mAll available gcc/g++ versions to install are: [\u001b[36m${AVAILABLE_VERSIONS//$'\n'/, }\x1B[01;93m];\n\u001b[0m"

if [ "${VERSION}" == "latest" ]; then
  VERSION="${LATEST_VERSION}"
elif ! echo "${AVAILABLE_VERSIONS}" | grep -q "^${VERSION}$"; then
  echo -e "\x1B[31m[ERROR] gcc-${VERSION} g++-${VERSION} not found.\n"
  exit 1
fi

if [ "${VERSION}" == "${CURRENT_VERSION}" ]; then
  echo -e "\x1B[01;93mThe required version to install is already the current version.\nNothing to do here.\n"
  exit 0
fi

echo -e "\x1B[01;93mInstalling or updating gcc-${VERSION} g++-${VERSION}...\n\u001b[0m"

apt install gcc-"${VERSION}" g++-"${VERSION}" -y
add-alternative "${VERSION}"

echo -e "\u001b[35m\n--\n\u001b[0m"

set-alternative "${VERSION}"

echo -e "\x1B[01;93m\nNow, the updated version of gcc/g++ compiler is:\n"
echo -e "\u001b[0m$(gcc --version)"
echo -e "\u001b[35m\n--\n\u001b[0m"

echo -e "You can switch between the different versions with:\n"
echo -e "\x1B[01;93m   $ update-alternatives --config gcc"
echo -e "\x1B[01;93m   $ update-alternatives --config g++\n"
