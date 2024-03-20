package client

import "github.com/robocin/ssl-core/match-bff/internal/domain"

type Clienter interface {
	GetConn() interface{}
	Close() error
	GetMatch() (domain.Match, error)
}
