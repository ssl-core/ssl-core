package main

import (
	"fmt"
	"net/http"
	"robocin-ines/match-bff/internal/handlers"
	"robocin-ines/match-bff/internal/services"
)

func main() {
	matchService := services.NewMatchService()
	http.HandleFunc("/match", func(w http.ResponseWriter, r *http.Request) {
		matchHandler := handlers.NewMatchHandler(*matchService)
		matchHandler.GetMatch(w, r)
	})

	fmt.Println("Server started on port 8080")

	err := http.ListenAndServe(":8080", nil)

	if err != nil {
		panic(err)
	}
}
