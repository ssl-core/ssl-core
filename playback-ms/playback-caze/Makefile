run:
	@go run cmd/main.go $(ARGS)

build:
	@go build -o bin/playback cmd/main.go

build-linux:
	@CGO_ENABLED=1 GOOS=linux GOARCH=amd64 go build -o bin/playback cmd/main.go

setup:
	@go mod download
	@bash ../../common/golang/scripts/compile_proto.sh github.com/robocin/ssl-core/playback-ms/playback-caze
