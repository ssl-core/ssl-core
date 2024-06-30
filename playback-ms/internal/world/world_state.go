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
	latest_sample      *entity.Sample
	latest_sample_mu   sync.RWMutex
	unsaved_samples    []*entity.Sample
	unsaved_samples_mu sync.RWMutex
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

func (ws *WorldState) updateLatestSample(identifier []byte, message []byte) {
	var topic string = string(identifier)
	switch topic {
	case service_discovery.GetInstance().GetDetectionWrapperTopic():
		var detectionWrapperProto perception.DetectionWrapper
		if err := proto.Unmarshal(message, &detectionWrapperProto); err != nil {
			fmt.Errorf("failed to unmarshal DetectionWrapper: %v", err)
		}
		ws.latest_sample.Detection = entity.NewDetection(detectionWrapperProto.Detection)
	default:
		fmt.Errorf("unexpected topic for ZmqMultipartDatagram: %s", topic)
	}
}

func (ws *WorldState) UpdateFromDatagram(datagram *network.ZmqMultipartDatagram) {
	ws.latest_sample_mu.Lock()
	ws.unsaved_samples_mu.Lock()

	defer ws.latest_sample_mu.Unlock()
	defer ws.unsaved_samples_mu.Unlock()

	ws.updateLatestSample(datagram.Identifier, datagram.Message)
	ws.unsaved_samples = append(ws.unsaved_samples, ws.latest_sample)
}

func (ws *WorldState) GetLatestSample() (*entity.Sample, error) {
	ws.latest_sample_mu.RLock()
	defer ws.latest_sample_mu.RUnlock()
	if ws.latest_sample == nil {
		return nil, fmt.Errorf("latest_sample is nil")
	}
	return ws.latest_sample, nil
}

func (ws *WorldState) GetUnsavedSamples() []*entity.Sample {
	ws.unsaved_samples_mu.RLock()
	defer ws.unsaved_samples_mu.RUnlock()
	return ws.unsaved_samples
}
