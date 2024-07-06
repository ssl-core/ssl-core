package entity

import "github.com/robocin/ssl-core/playback-ms/pkg/pb/referee"

type GameEventsProposal struct {
	ProposalId  string       `json:"proposal_id"`
	GameEvents  []*GameEvent `json:"game_events"`
	WasAccepted bool         `json:"was_accepted"`
}

func NewGameEventsProposalsFromRefereeGameEventsProposals(gameEventsProposals []*referee.GameStatus_GameEventsProposal) []GameEventsProposal {
	var proposals []GameEventsProposal
	for _, gameEventsProposal := range gameEventsProposals {
		proposals = append(proposals, GameEventsProposal{
			ProposalId:  gameEventsProposal.ProposalId,
			GameEvents:  gameEventsProposal.GameEvents,
			WasAccepted: gameEventsProposal.WasAccepted,
		})
	}
	return proposals
}
