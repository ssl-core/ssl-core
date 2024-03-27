# Colors
GREEN="\033[0;32m"
NC="\033[0m"

# Directories
PROTO_DIR="/tmp/protocols"
BUILD_DIR="./pkg"
GO_MODULE="github.com/robocin/ssl-core/match-bff"

# Variables
FLAGS=""

# Create folder if not exists
mkdir -p ${BUILD_DIR}

# Loop over each .proto file and append flags
while IFS= read -r -d '' file; do
    relative_filepath="${file#${PROTO_DIR}/}"
    relative_path="$(dirname "${relative_filepath#protocols/}")"
    FLAGS+="--go_opt=M${relative_filepath}=${GO_MODULE}/pkg/pb/${relative_path} --go-grpc_opt=M${relative_filepath}=${GO_MODULE}/pkg/pb/${relative_path} "
done < <(find "${PROTO_DIR}" -name '*.proto' -print0)

# Call protoc with the generated flags
protoc -I "${PROTO_DIR}" --go_out="${BUILD_DIR}" --go-grpc_out="${BUILD_DIR}" \
       --go_opt=paths=source_relative --go-grpc_opt=paths=source_relative \
       ${FLAGS} \
       $(find "${PROTO_DIR}" -name '*.proto')

# Move the generated files to the pb directory
rm -rf ${BUILD_DIR}/pb
mv ${BUILD_DIR}/protocols ${BUILD_DIR}/pb

# Print message
echo -e "${GREEN}✓ Protobufs compiled!${NC}"
