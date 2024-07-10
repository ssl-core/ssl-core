package service_discovery

const (
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
