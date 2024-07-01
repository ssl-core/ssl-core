package db

import "time"

type DBClient interface {
	Get(time.Time) (interface{}, error)
	GetChunk(time.Time, time.Time) (interface{}, error)
	Set(time.Time, interface{}) error
	Clear() error
}
