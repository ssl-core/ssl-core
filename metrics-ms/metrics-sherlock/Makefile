run:
	@go run cmd/main.go

build:
	@go build -o bin/metrics cmd/main.go

build-linux:
	@CGO_ENABLED=1 GOOS=linux GOARCH=amd64 go build -o bin/metrics cmd/main.go

setup:
	@go mod download
	@bash ../../common/golang/scripts/compile_proto.sh
