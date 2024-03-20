package server

import (
	"fmt"
	"net/http"
)

type HttpServer struct {
	address   string
	websocket *WebsocketServer
	router    *http.ServeMux
}

func NewHttpServer(address string, websocket *WebsocketServer) *HttpServer {
	router := http.NewServeMux()
	server := &HttpServer{
		address:   address,
		websocket: websocket,
		router:    router,
	}

	server.registerRoutes()
	server.registerWebsocket()

	return server
}

func (hs *HttpServer) GetRouter() *http.ServeMux {
	return hs.router
}

func (hs *HttpServer) Serve() error {
	return http.ListenAndServe(hs.address, hs.router)
}

func (hs *HttpServer) registerRoutes() {
	hs.router.HandleFunc("POST /todos", func(w http.ResponseWriter, r *http.Request) {
		fmt.Println("create a todo")
	})

	hs.router.HandleFunc("GET /todos", func(w http.ResponseWriter, r *http.Request) {
		fmt.Println("get all todos")
	})
}

func (hs *HttpServer) registerWebsocket() {
	hs.router.Handle("/ws", hs.websocket.GetHandler())
}
