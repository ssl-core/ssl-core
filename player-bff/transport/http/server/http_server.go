package server

import (
	"net/http"

	"github.com/robocin/ssl-core/player-bff/api/handler"
)

type HttpServer struct {
	address string
	router  *http.ServeMux
}

func NewHttpServer(address string) *HttpServer {
	router := http.NewServeMux()
	server := &HttpServer{
		address: address,
		router:  router,
	}

	server.registerHandlers()

	return server
}

func (hs *HttpServer) GetRouter() *http.ServeMux {
	return hs.router
}

func (hs *HttpServer) Serve() error {
	return http.ListenAndServe(hs.address, hs.router)
}

func (hs *HttpServer) registerHandlers() {
	hs.router.HandleFunc("GET /health", handler.HealthHandler())
}
