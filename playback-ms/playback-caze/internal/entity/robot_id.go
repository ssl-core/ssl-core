package entity

import (
	"encoding/json"

	"github.com/robocin/ssl-core/playback-ms/pkg/pb/common"
)

type RobotId struct {
	Id    uint32
	Color RobotColor
}

func NewRobotIdFromCommonRobotId(commonRobotId *common.RobotId) RobotId {
	return RobotId{
		Id:    commonRobotId.Number,
		Color: RobotColor(commonRobotId.Color),
	}
}

func (ri *RobotId) ToJson() ([]byte, error) {
	return json.Marshal(ri)
}

func (ri *RobotId) ToProto() *common.RobotId {
	return &common.RobotId{
		Number: ri.Id,
		Color:  ri.Color.ToProto(),
	}
}

type RobotColor int

const (
	Blue        RobotColor = iota + 1 // EnumIndex = 1
	Yellow                            // EnumIndex = 2
	Unspecified                       // EnumIndex = 3
)

func (rc RobotColor) String() string {
	return [...]string{"Blue", "Yellow", "Unspecified"}[rc-1]
}

func (rc RobotColor) EnumIndex() int {
	return int(rc)
}

func (rc *RobotColor) ToJson() ([]byte, error) {
	return json.Marshal(rc)
}

func (rc *RobotColor) ToProto() common.RobotId_Color {
	switch *rc {
	case Blue:
		return common.RobotId_COLOR_BLUE
	case Yellow:
		return common.RobotId_COLOR_YELLOW
	default:
		return common.RobotId_COLOR_UNSPECIFIED
	}
}
