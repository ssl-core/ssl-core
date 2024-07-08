package entity

import (
	"encoding/json"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/third_party/detection"
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

func NewRawDetectionFromRawPackets(raw_packets_pb []*detection.SSL_WrapperPacket) *RawDetection {
	cameras := make([]RawCamera, len(raw_packets_pb))
	for index := range raw_packets_pb {
		frame := raw_packets_pb[index].Detection
		if frame != nil {
			camera := NewRawCameraFromSSLDetectionFrame(frame)
			cameras = append(cameras, *camera)
		}
	}

	return &RawDetection{
		Cameras: cameras,
	}
}

func (rd *RawDetection) ToJson() ([]byte, error) {
	return json.Marshal(rd)
}

func (rd *RawDetection) ToProto() *playback.RawDetection {
	cameras := make([]*playback.RawDetection_Camera, len(rd.Cameras))
	for i, raw_camera := range rd.Cameras {
		cameras[i] = raw_camera.ToProto()
	}

	return &playback.RawDetection{
		Cameras: cameras,
	}
}
