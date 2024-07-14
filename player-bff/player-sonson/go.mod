module github.com/robocin/ssl-core/player-bff/player-sonson

go 1.22.0

require (
	github.com/gorilla/websocket v1.5.3
	github.com/robocin/ssl-core/protocols v0.0.0-00010101000000-000000000000
	google.golang.org/grpc v1.65.0
	google.golang.org/protobuf v1.34.2
)

require (
	golang.org/x/net v0.27.0 // indirect
	golang.org/x/sys v0.22.0 // indirect
	golang.org/x/text v0.16.0 // indirect
	google.golang.org/genproto/googleapis/rpc v0.0.0-20240711142825-46eb208f015d // indirect
)

replace github.com/robocin/ssl-core/protocols => /tmp/ssl-core/golang/protocols
