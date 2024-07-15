module github.com/robocin/ssl-core/metrics-ms/metrics-sherlock

go 1.22

require (
	github.com/influxdata/influxdb-client-go/v2 v2.13.0
	github.com/robocin/ssl-core/common/golang v0.0.0-00010101000000-000000000000
	github.com/robocin/ssl-core/protocols v0.0.0-00010101000000-000000000000
	github.com/zeromq/goczmq v4.1.0+incompatible
	google.golang.org/protobuf v1.34.2
)

require (
	github.com/apapsch/go-jsonmerge/v2 v2.0.0 // indirect
	github.com/google/uuid v1.6.0 // indirect
	github.com/influxdata/line-protocol v0.0.0-20210922203350-b1ad95c89adf // indirect
	github.com/oapi-codegen/runtime v1.1.1 // indirect
	github.com/stretchr/testify v1.9.0 // indirect
	golang.org/x/net v0.27.0 // indirect
)

replace github.com/robocin/ssl-core/common/golang => ../../common/golang

replace github.com/robocin/ssl-core/protocols => /tmp/ssl-core/golang/protocols
