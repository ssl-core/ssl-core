package entity

import (
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
)

type RawDetection struct {
	Cameras []RawCamera `json:"cameras"`
}

func NewRawDetection(raw_detection_pb *playback.RawDetection) RawDetection {
	cameras := make([]RawCamera, len(raw_detection_pb.Cameras))
	for i, raw_camera_pb := range raw_detection_pb.Cameras {
		cameras[i] = NewRawCamera(raw_camera_pb)
	}

	return RawDetection{
		Cameras: cameras,
	}
}
