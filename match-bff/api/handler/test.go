package handler

import (
	"net/http"

	"github.com/robocin/ssl-core/match-bff/internal/client"
)

func TestHandler(client client.Clienter) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		w.WriteHeader(http.StatusOK)
		w.Header().Set("Content-Type", "application/json")
		w.Write([]byte(`{"message": "Hey! I'm a test route!"}`))
	}
}
