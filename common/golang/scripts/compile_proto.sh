#!/bin/bash

# Colors
GREEN="\033[0;32m"
NO_COLOR="\033[0m"

# Directories (relative tho the directory of the script)
PROTOS_DIR="../../../protocols"
GOLANG_FILES_DIR="/tmp/ssl-core/golang"
PROTOS_GOLANG_MODULE="github.com/robocin/ssl-core/protocols"

# Set the current working directory to the directory of the script
cd "$(dirname "$0")"

# Create folder if not exists
mkdir -p "${GOLANG_FILES_DIR}"

# Variables
FLAGS=""

# Loop over each .proto file and append flags
while IFS= read -r -d '' file; do
    relative_filepath="${file#${PROTOS_DIR}/}"
    relative_path="$(dirname "${relative_filepath#protocols/}")"
    FLAGS+="--go_opt=M${relative_filepath}=${PROTOS_GOLANG_MODULE}/${relative_path} --go-grpc_opt=M${relative_filepath}=${PROTOS_GOLANG_MODULE}/${relative_path} "
done < <(find "${PROTOS_DIR}" -name '*.proto' -print0)

# Call protoc with the generated flags
protoc -I "${PROTOS_DIR}" \
       --go_out="${GOLANG_FILES_DIR}" \
       --go-grpc_out="${GOLANG_FILES_DIR}" \
       --go_opt=paths=source_relative \
       --go-grpc_opt=paths=source_relative \
       ${FLAGS} \
       $(find "${PROTOS_DIR}" -name '*.proto')

pushd "${GOLANG_FILES_DIR}/protocols"|| exit -1
go mod init "${PROTOS_GOLANG_MODULE}"
go mod tidy -go=1.22
popd || exit -1

# Print successful message
echo -e "${GREEN}✓ Protobufs compiled!${NO_COLOR}"
