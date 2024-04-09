package entity

import (
	"github.com/robocin/ssl-core/match-bff/internal/util"
	"github.com/robocin/ssl-core/match-bff/pkg/pb/vision"
)

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
	serialId := util.SetDefaultIfNil(field.SerialId, 0)
	length := util.SetDefaultIfNil(field.Length, 0) / 1000
	width := util.SetDefaultIfNil(field.Width, 0) / 1000
	goalDepth := util.SetDefaultIfNil(field.GoalDepth, 0) / 1000
	goalWidth := util.SetDefaultIfNil(field.GoalWidth, 0) / 1000
	penaltyAreaDepth := util.SetDefaultIfNil(field.PenaltyAreaDepth, 0) / 1000
	penaltyAreaWidth := util.SetDefaultIfNil(field.PenaltyAreaWidth, 0) / 1000
	boundaryWidth := util.SetDefaultIfNil(field.BoundaryWidth, 0) / 1000
	goalCenterToPenaltyMark := util.SetDefaultIfNil(field.GoalCenterToPenaltyMark, 0) / 1000

	return Field{
		SerialId:                serialId,
		Length:                  length,
		Width:                   width,
		GoalDepth:               goalDepth,
		GoalWidth:               goalWidth,
		PenaltyAreaDepth:        penaltyAreaDepth,
		PenaltyAreaWidth:        penaltyAreaWidth,
		BoundaryWidth:           boundaryWidth,
		GoalCenterToPenaltyMark: goalCenterToPenaltyMark,
	}
}
