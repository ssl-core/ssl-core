package service_discovery

const (
	GatewayThirdPartyAddress      = "ipc:///tmp/.ssl-core/gateway.ipc"
	GatewayThirdPartyVisionTopic  = "vision-third-party"
	GatewayThirdPartyTrackedTopic = "tracked-third-party"
	GatewayThirdPartyRefereeTopic = "referee-third-party"

	PerceptionAddress               = "ipc:///tmp/.ssl-core/perception.ipc"
	PerceptionDetectionTopic        = "detection"
	PerceptionDetectionWrapperTopic = "wrapper"

	RefereeAddress         = "ipc:///tmp/.ssl-core/referee.ipc"
	RefereeGameStatusTopic = "game-status"

	PlaybackAddress     = "ipc:///tmp/.ssl-core/playback.ipc"
	PlaybackSampleTopic = "sample"

	GatewayReplayChunckAddress = "ipc:///tmp/.ssl-core/replay.ipc"
	GatewayLivestreamTopic     = "sample"
)
