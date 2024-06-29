package service_discovery

import (
	"fmt"
	"sync"
)

const (
	perceptionAddress     = "ipc:///tmp/perception.ipc"
	detectionWrapperTopic = "wrapper"
	refereeAddress        = "ipc:///tmp/referee.ipc"
	refereeTopic          = "referee"

	chunkAddress = "ipc:///tmp/dr.ipc"
)

var lock = &sync.Mutex{}

type ServiceDiscovery struct {
}

var instance *ServiceDiscovery

func GetInstance() *ServiceDiscovery {
	if instance == nil {
		lock.Lock()
		defer lock.Unlock()
		if instance == nil {
			fmt.Println("Creating ServiceDiscovery instance now.")
			instance = &ServiceDiscovery{}
		} else {
			fmt.Println("ServiceDiscovery instance already created.")
		}
	} else {
		fmt.Println("ServiceDiscovery instance already created.")
	}

	return instance
}

func (sd *ServiceDiscovery) GetPerceptionAddress() string {
	return perceptionAddress
}

func (sd *ServiceDiscovery) GetDetectionWrapperTopic() string {
	return detectionWrapperTopic
}

func (sd *ServiceDiscovery) GetRefereeAddress() string {
	return refereeAddress
}

func (sd *ServiceDiscovery) GetRefereeTopic() string {
	return refereeTopic
}

func (sd *ServiceDiscovery) GetChunkAddress() string {
	return chunkAddress
}
