module github.com/robocin/ssl-core/gateway/gateway-augusto

go 1.22

require (
	github.com/robocin/ssl-core/common/golang v0.0.0-00010101000000-000000000000
	github.com/robocin/ssl-core/protocols v0.0.0-00010101000000-000000000000
	google.golang.org/grpc v1.65.0
	google.golang.org/protobuf v1.34.2
)

require (
	github.com/golang/protobuf v1.5.4
	github.com/zeromq/goczmq v4.1.0+incompatible // indirect
	golang.org/x/net v0.27.0 // indirect
	golang.org/x/sys v0.22.0 // indirect
	golang.org/x/text v0.16.0 // indirect
	google.golang.org/genproto/googleapis/rpc v0.0.0-20240711142825-46eb208f015d // indirect
)

replace github.com/robocin/ssl-core/common/golang => ../../common/golang

replace github.com/robocin/ssl-core/protocols => /tmp/ssl-core/golang/protocols
