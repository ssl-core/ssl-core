run:
	@go run cmd/main.go

build:
	@go build -o bin/player cmd/main.go

build-linux:
	@CGO_ENABLED=0 GOOS=linux GOARCH=amd64 go build -o bin/player cmd/main.go

setup:
	@go mod download
	@bash ../../common/golang/scripts/compile_proto.sh github.com/robocin/ssl-core/player-bff/player-sonson
