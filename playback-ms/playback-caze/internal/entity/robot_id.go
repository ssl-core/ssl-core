package entity

import "encoding/json"

type RobotId struct {
	Id    uint32
	Color RobotColor
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
