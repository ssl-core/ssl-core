run:
	@go run cmd/main.go $(ARGS)

build:
	@go build -o bin/playback-ms cmd/main.go

build-linux:
	@CGO_ENABLED=1 GOOS=linux GOARCH=amd64 go build -o bin/playback-ms cmd/main.go

setup:
	@go mod download

compile-proto:
	@bash scripts/compile_proto.sh
