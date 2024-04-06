package domain

import "github.com/robocin/ssl-core/match-bff/pkg/pb/vision"

type Field struct {
	SerialId                uint64  `json:"serial_id"`
	Length                  float32 `json:"length"`
	Width                   float32 `json:"width"`
	GoalDepth               float32 `json:"goal_depth"`
	GoalWidth               float32 `json:"goal_width"`
	PenaltyAreaDepth        float32 `json:"penalty_area_depth"`
	PenaltyAreaWidth        float32 `json:"penalty_area_width"`
	BoundaryWidth           float32 `json:"boundary_width"`
	GoalCenterToPenaltyMark float32 `json:"goal_center_to_penalty_mark"`
}

func NewFieldFromProto(field *vision.Field) Field {
	return Field{
		SerialId:                field.SerialId,
		Length:                  field.Length,
		Width:                   field.Width,
		GoalDepth:               field.GoalDepth,
		GoalWidth:               field.GoalWidth,
		PenaltyAreaDepth:        field.PenaltyAreaDepth,
		PenaltyAreaWidth:        field.PenaltyAreaWidth,
		BoundaryWidth:           field.BoundaryWidth,
		GoalCenterToPenaltyMark: field.GoalCenterToPenaltyMark,
	}
}
