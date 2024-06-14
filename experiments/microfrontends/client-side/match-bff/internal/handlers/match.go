package handlers

import (
	"encoding/json"
	"net/http"
	"robocin-ines/match-bff/internal/services"
	"strconv"
)

type MatchHandler struct {
	MatchService services.MatchService
}

func NewMatchHandler(matchService services.MatchService) *MatchHandler {
	return &MatchHandler{
		MatchService: matchService,
	}
}

func (h *MatchHandler) GetMatch(w http.ResponseWriter, r *http.Request) {
	id := r.URL.Query().Get("id")
	timestamp, _ := strconv.ParseInt(r.URL.Query().Get("timestamp"), 10, 64)

	if id == "" {
		http.Error(w, "id is required", http.StatusBadRequest)
		return
	}

	if timestamp == 0 {
		http.Error(w, "timestamp is required", http.StatusBadRequest)
		return
	}

	frames := h.MatchService.GetMatchChunk(id, timestamp)

	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(frames)
}
