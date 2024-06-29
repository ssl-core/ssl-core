package latest_sample

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"google.golang.org/protobuf/proto"
)

type LatestSample struct {
	sample *entity.Sample
}

var instance *LatestSample

var lock = &sync.Mutex{}

func GetInstance() *LatestSample {
	lock.Lock()
	defer lock.Unlock()
	if instance == nil {
		instance = &LatestSample{}
	}
	return instance
}

func (ls *LatestSample) UpdateFromDatagram(datagram network.ZmqMultipartDatagram) {
	ls.updateProperty(datagram.Identifier, datagram.Message)
}

func (ls *LatestSample) updateProperty(identifier []byte, message []byte) {
	var topic string = string(identifier)
	switch topic {
	case "detection":
		var detectionProto playback.Detection
		if err := proto.Unmarshal(message, &detectionProto); err != nil {
			fmt.Errorf("failed to unmarshal detection: %v", err)
		}
		ls.sample.Detection = entity.NewDetection(&detectionProto)
	default:
		fmt.Errorf("unexpected topic for ZmqMultipartDatagram: %s", topic)
	}
}

func (ls *LatestSample) GetSample() (*entity.Sample, error) {
	lock.Lock()
	defer lock.Unlock()
	if ls.sample == nil {
		return nil, fmt.Errorf("sample is nil")
	}
	return ls.sample, nil
}
