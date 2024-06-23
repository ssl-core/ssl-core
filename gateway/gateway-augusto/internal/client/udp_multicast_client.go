package client

import (
	"fmt"
	"net"
)

const UDP = "udp"

type UdpMulticastClient struct {
	address *net.UDPAddr
	conn    *net.UDPConn
	size    int
	proxy   chan<- []byte
}

func NewUdpMulticastClient(address string, size int, proxy chan<- []byte) *UdpMulticastClient {
	addr, err := net.ResolveUDPAddr(UDP, address)
	if err != nil {
		panic(err)
	}

	conn, err := net.ListenMulticastUDP(UDP, nil, addr)
	if err != nil {
		panic(err)
	}

	return &UdpMulticastClient{
		address: addr,
		conn:    conn,
		size:    size,
		proxy:   proxy,
	}
}

func (server *UdpMulticastClient) Listen() {
	buffer := make([]byte, server.size)
	for {
		bytes, _, err := server.conn.ReadFromUDP(buffer)
		if err != nil {
			continue
		}
		server.proxy <- buffer[:bytes]
		fmt.Println(string(buffer[:bytes]))
	}
}

func (server *UdpMulticastClient) Close() {
	server.conn.Close()
}
