package mappers

import (
	"github.com/robocin/ssl-core/protocols/playback"
	"github.com/robocin/ssl-core/protocols/third_party/game_controller"
)

func TrackedDetectionMapper(input []*game_controller.TrackerWrapperPacket) *playback.TrackedDetection {
	if input == nil {
		return nil
	}

	if len(input) == 0 {
		return nil
	}

	for i := len(input) - 1; i >= 0; i-- {
		packet := input[i]
		if packet.GetTrackedFrame() != nil {
			return &playback.TrackedDetection{
				SerialId: packet.GetTrackedFrame().GetFrameNumber(),
			}
		}
	}

	return nil
}
