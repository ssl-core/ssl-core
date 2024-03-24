package client

import (
	"log"

	"github.com/zeromq/goczmq"
)

type ZmqClient struct {
	address string
	conn    *goczmq.Sock
}

func NewZmqClient(address string) *ZmqClient {
	conn, err := goczmq.NewReq(address)
	if err != nil {
		log.Fatalf("Failed to listen: %v", err)
	}

	return &ZmqClient{
		address: address,
		conn:    conn,
	}
}

func (zc *ZmqClient) GetConn() *goczmq.Sock {
	return zc.conn
}

func (zc *ZmqClient) Close() error {
	zc.conn.Destroy()
	return nil
}
