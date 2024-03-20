package client

import (
	"log"

	"github.com/robocin/ssl-core/match-bff/internal/domain"
	pb "github.com/robocin/ssl-core/match-bff/pkg/pb"
	"github.com/zeromq/goczmq"
	"google.golang.org/protobuf/proto"
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

func (zc *ZmqClient) GetMatch() (domain.Match, error) {
	err := zc.conn.SendFrame([]byte("test"), goczmq.FlagNone)
	if err != nil {
		return domain.Match{}, err
	}

	msg, err := zc.conn.RecvMessage()
	if err != nil {
		return domain.Match{}, err
	}

	response := &pb.Match{}
	if err := proto.Unmarshal(msg[0], response); err != nil {
		return domain.Match{}, err
	}

	match := domain.Match{
		Id:   response.Id,
		Name: response.Name,
	}
	return match, nil
}
