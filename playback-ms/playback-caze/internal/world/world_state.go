package world

import (
	"fmt"
	"sync"

	"github.com/robocin/ssl-core/playback-ms/internal/entity"
	"github.com/robocin/ssl-core/playback-ms/internal/service_discovery"
	"github.com/robocin/ssl-core/playback-ms/network"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/perception"
	"google.golang.org/protobuf/proto"
)

type WorldState struct {
	latestSample          *entity.Sample
	latestSampleMutex     sync.RWMutex
	unsavedSamplesChannel chan entity.Sample
}

var instance *WorldState

var lock sync.Mutex

func GetInstance() *WorldState {
	lock.Lock()
	defer lock.Unlock()
	if instance == nil {
		instance = &WorldState{}
	}
	return instance
}

func (ws *WorldState) Init(unsavedSamplesChannel chan entity.Sample) {
	ws.unsavedSamplesChannel = unsavedSamplesChannel
}

func (ws *WorldState) updateLatestSample(identifier []byte, message []byte) {
	var topic string = string(identifier)
	switch topic {
	case service_discovery.GetInstance().GetDetectionWrapperTopic():
		var detectionWrapperProto perception.DetectionWrapper
		if err := proto.Unmarshal(message, &detectionWrapperProto); err != nil {
			fmt.Printf("failed to unmarshal DetectionWrapper: %v", err)
			return
		}
		ws.latestSample.Detection = entity.NewDetection(detectionWrapperProto.Detection)
	default:
		fmt.Printf("unexpected topic for ZmqMultipartDatagram: %s", topic)
	}
}

func (ws *WorldState) UpdateFromDatagram(datagram *network.ZmqMultipartDatagram) {
	ws.latestSampleMutex.Lock()

	defer ws.latestSampleMutex.Unlock()

	ws.updateLatestSample(datagram.Identifier, datagram.Message)
	ws.unsavedSamplesChannel <- (*ws.latestSample)
}

func (ws *WorldState) GetLatestSample() (*entity.Sample, error) {
	ws.latestSampleMutex.RLock()
	defer ws.latestSampleMutex.RUnlock()
	if ws.latestSample == nil {
		return nil, fmt.Errorf("latestSample is nil")
	}
	return ws.latestSample, nil
}
