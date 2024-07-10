package mappers

import (
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/playback"
	"github.com/robocin/ssl-core/playback-ms/pkg/pb/referee"
)

func GameStatusMapper(input *referee.GameStatus) *playback.GameStatus {
	if input == nil {
		return nil
	}

	return &playback.GameStatus{
		SourceId:               input.SourceId,
		Description:            input.Description,
		Timestamp:              input.Timestamp,
		MatchType:              input.MatchType,
		HomeTeam:               teamMapper(input.HomeTeam),
		AwayTeam:               teamMapper(input.AwayTeam),
		GameStage:              input.GameStage,
		GameStageTimeLeft:      input.GameStageTimeLeft,
		TotalCommandsIssued:    input.TotalCommandsIssued,
		CommandIssuedTimestamp: input.CommandIssuedTimestamp,
		Command:                input.Command,
		NextCommand:            input.NextCommand,
		GameEvents:             input.GameEvents,
		GameEventsProposals:    gameEventsProposalsMapper(input.GameEventsProposals),
	}
}

func teamMapper(input *referee.GameStatus_Team) *playback.GameStatus_Team {
	if input == nil {
		return nil
	}

	return &playback.GameStatus_Team{
		Name:                                   input.Name,
		Score:                                  input.Score,
		RobotIds:                               input.RobotIds,
		GoalkeeperId:                           input.GoalkeeperId,
		YellowCards:                            input.YellowCards,
		TimeToExpireActiveYellowCards:          input.TimeToExpireActiveYellowCards,
		RedCards:                               input.RedCards,
		TimeoutsLeft:                           input.TimeoutsLeft,
		TotalTimeoutTimeLeft:                   input.TotalTimeoutTimeLeft,
		TotalFoulsCommitted:                    input.TotalFoulsCommitted,
		ConsecutiveBallPlacementFailures:       input.ConsecutiveBallPlacementFailures,
		IsBallPlacementEnabled:                 input.IsBallPlacementEnabled,
		HasBallPlacementFailuresReachedMaximum: input.HasBallPlacementFailuresReachedMaximum,
		MaximumAllowedRobots:                   input.MaximumAllowedRobots,
		IsRobotSubstitutionRequested:           input.IsRobotSubstitutionRequested,
		IsRobotSubstitutionAllowed:             input.IsRobotSubstitutionAllowed,
		RobotSubstitutionsLeft:                 input.RobotSubstitutionsLeft,
		RobotSubstitutionTimeLeft:              input.RobotSubstitutionTimeLeft,
	}
}

func gameEventsProposalsMapper(input []*referee.GameStatus_GameEventsProposal) []*playback.GameStatus_GameEventsProposal {
	output := make([]*playback.GameStatus_GameEventsProposal, 0)

	for _, proposal := range input {
		output = append(output, &playback.GameStatus_GameEventsProposal{
			ProposalId:  proposal.ProposalId,
			GameEvents:  proposal.GameEvents,
			WasAccepted: proposal.WasAccepted,
		})
	}

	return output
}
