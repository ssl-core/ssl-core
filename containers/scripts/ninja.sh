#!/bin/bash

function is_root {
  [ "${EUID:-$(id -u)}" -eq 0 ];
}

if ! is_root; then
  echo -e "\x1B[31m[ERROR] This script requires root privileges."
  exit 1
fi

VERSION="${1}"

if [ -z "${VERSION}" ]; then
  echo -e "\x1B[31m[ERROR] No version specified."
  exit 1
fi

echo -e "\x1B[01;93mInstalling or updating ninja...\n\u001b[0m"

wget https://github.com/ninja-build/ninja/releases/download/v$VERSION/ninja-linux.zip -O /tmp/ninja.zip

unzip /tmp/ninja.zip -d /tmp
mv /tmp/ninja /usr/bin/
rm -rf /tmp/ninja.zip /tmp/ninja

chmod +x /usr/bin/ninja

chown "${CURRENT_USER}":"${CURRENT_USER}" /usr/bin/ninja # changes the owner of the directory to the current user
