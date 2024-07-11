package mappers

import (
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/perception"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
)

func FieldMapper(input *perception.Field) *playback.Field {
	if input == nil {
		return nil
	}
	return &playback.Field{
		Length:                  input.Length,
		Width:                   input.Width,
		GoalDepth:               input.GoalDepth,
		GoalWidth:               input.GoalWidth,
		PenaltyAreaDepth:        input.PenaltyAreaDepth,
		PenaltyAreaWidth:        input.PenaltyAreaWidth,
		BoundaryWidth:           input.BoundaryWidth,
		GoalCenterToPenaltyMark: input.GoalCenterToPenaltyMark,
	}
}
