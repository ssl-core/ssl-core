run:
	@go run cmd/main.go $(ARGS)

build:
	@go build -o bin/playback cmd/main.go

build-linux:
	@CGO_ENABLED=1 GOOS=linux GOARCH=amd64 go build -o bin/playback cmd/main.go

setup:
	@bash ../../common/golang/scripts/compile_proto.sh
	@go mod download
