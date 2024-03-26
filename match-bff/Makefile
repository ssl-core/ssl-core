run:
	@go run cmd/main.go

setup:
	@bash scripts/setup-protoc.sh
	@bash scripts/setup-zmq.sh
	@bash scripts/compile-proto.sh
	@go mod download

compile-proto:
	@bash scripts/compile-proto.sh
