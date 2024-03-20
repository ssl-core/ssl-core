run:
	@go run cmd/main.go

compile-proto:
	@protoc -I=./proto/v1 --go_out=./pkg/pb --go-grpc_out=./pkg/pb --go_opt=paths=source_relative --go-grpc_opt=paths=source_relative ./proto/v1/*.proto
