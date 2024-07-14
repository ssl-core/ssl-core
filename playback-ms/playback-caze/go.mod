module github.com/robocin/ssl-core/playback-ms/playback-caze

go 1.22

require (
	github.com/redis/go-redis/v9 v9.5.4
	github.com/robocin/ssl-core/common/golang v0.0.0-00010101000000-000000000000
	github.com/robocin/ssl-core/protocols v0.0.0-00010101000000-000000000000
	github.com/zeromq/goczmq v4.1.0+incompatible
	google.golang.org/grpc v1.65.0
	google.golang.org/protobuf v1.34.2
)

require (
	github.com/cespare/xxhash/v2 v2.3.0 // indirect
	github.com/dgryski/go-rendezvous v0.0.0-20200823014737-9f7001d12a5f // indirect
	golang.org/x/net v0.27.0 // indirect
	golang.org/x/sys v0.22.0 // indirect
	golang.org/x/text v0.16.0 // indirect
	google.golang.org/genproto/googleapis/rpc v0.0.0-20240711142825-46eb208f015d // indirect
)

replace github.com/robocin/ssl-core/common/golang => ../../common/golang

replace github.com/robocin/ssl-core/protocols => /tmp/ssl-core/golang/protocols
