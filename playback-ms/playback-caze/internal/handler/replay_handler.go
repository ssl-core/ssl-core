package handler

import (
	"fmt"

	"github.com/robocin/ssl-core/common/golang/network"
	"github.com/robocin/ssl-core/protocols/gateway"
	"google.golang.org/protobuf/proto"
)

type ReplayHandler struct {
}

func NewReplayHandler() *ReplayHandler {
	return &ReplayHandler{}
}

func (rh *ReplayHandler) Process(datagram *network.ZmqMultipartDatagram) (*gateway.GetReplayChunkRequest, error) {
	var replayRequest gateway.GetReplayChunkRequest
	if err := proto.Unmarshal(datagram.Message, &replayRequest); err == nil {
		return &replayRequest, nil
	}
	return nil, fmt.Errorf("datagram not decodable by ReplayHandler")
}
