# Directories
PROTO_DIR = ../../../protocols
BUILD_DIR = .

run:
	@go run main.go

compile:
	@protoc -I $(PROTO_DIR) --go_out=${BUILD_DIR} --go-grpc_out=${BUILD_DIR} \
													--go_opt=paths=source_relative --go-grpc_opt=paths=source_relative \
													$$(find ${PROTO_DIR} -name '*.proto')
	@rm -rf ${BUILD_DIR}/pb
	@mv ${BUILD_DIR}/protocols ${BUILD_DIR}/pb
	@echo "Protobufs compiled!"
