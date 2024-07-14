package entity

import (
	"github.com/robocin/ssl-core/player-bff/player-sonson/internal/util"
	"github.com/robocin/ssl-core/player-bff/player-sonson/pkg/pb/playback"
)

type Field struct {
	Length                  float32 `json:"length"`
	Width                   float32 `json:"width"`
	GoalDepth               float32 `json:"goal_depth"`
	GoalWidth               float32 `json:"goal_width"`
	PenaltyAreaDepth        float32 `json:"penalty_area_depth"`
	PenaltyAreaWidth        float32 `json:"penalty_area_width"`
	BoundaryWidth           float32 `json:"boundary_width"`
	GoalCenterToPenaltyMark float32 `json:"goal_center_to_penalty_mark"`
}

func NewField(field *playback.Field) Field {
	length := util.SetDefaultIfNil(field.GetLength(), 0) / 1000
	width := util.SetDefaultIfNil(field.GetWidth(), 0) / 1000
	goalDepth := util.SetDefaultIfNil(field.GetGoalDepth(), 0) / 1000
	goalWidth := util.SetDefaultIfNil(field.GetGoalWidth(), 0) / 1000
	penaltyAreaDepth := util.SetDefaultIfNil(field.GetPenaltyAreaDepth(), 0) / 1000
	penaltyAreaWidth := util.SetDefaultIfNil(field.GetPenaltyAreaWidth(), 0) / 1000
	boundaryWidth := util.SetDefaultIfNil(field.GetBoundaryWidth(), 0) / 1000
	goalCenterToPenaltyMark := util.SetDefaultIfNil(field.GetGoalCenterToPenaltyMark(), 0) / 1000

	return Field{
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
