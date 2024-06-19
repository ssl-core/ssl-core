run:
	@go run cmd/main.go

build:
	@go build -o bin/player-bff cmd/main.go

build-linux:
	@CGO_ENABLED=0 GOOS=linux GOARCH=amd64 go build -o bin/player-bff cmd/main.go

setup:
	@bash scripts/setup_protoc.sh
	@bash scripts/compile_proto.sh
	@go mod download

compile-proto:
	@bash scripts/compile_proto.sh
