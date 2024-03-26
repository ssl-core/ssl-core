package server

import (
	"net/http"

	"github.com/robocin/ssl-core/match-bff/api/handler"
	"github.com/robocin/ssl-core/match-bff/internal/client"
)

type HttpServer struct {
	address   string
	websocket *WebsocketServer
	router    *http.ServeMux
	client    client.Clienter
}

func NewHttpServer(address string, websocket *WebsocketServer, client client.Clienter) *HttpServer {
	router := http.NewServeMux()
	server := &HttpServer{
		address:   address,
		websocket: websocket,
		router:    router,
		client:    client,
	}

	server.registerHandlers()
	server.registerWebsocket()

	return server
}

func (hs *HttpServer) GetRouter() *http.ServeMux {
	return hs.router
}

func (hs *HttpServer) Serve() error {
	return http.ListenAndServe(hs.address, hs.router)
}

func (hs *HttpServer) registerHandlers() {
	hs.router.HandleFunc("GET /test", handler.TestHandler(hs.client))
}

func (hs *HttpServer) registerWebsocket() {
	hs.router.Handle("/ws", hs.websocket.GetHandler())
}
