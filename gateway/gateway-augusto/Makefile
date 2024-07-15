run:
	@go run cmd/main.go

build:
	@go build -o bin/gateway cmd/main.go

build-linux: # TODO(joseviccruz): CGO_ENABLED=0, i.e., static linking zmq.
	@CGO_ENABLED=1 GOOS=linux GOARCH=amd64 go build -o bin/gateway cmd/main.go

setup:
	@bash ../../common/golang/scripts/compile_proto.sh
	@go mod download
