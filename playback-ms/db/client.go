package db

type DBClient interface {
	Get(string) (interface{}, error)
	Set(string, interface{}) error
}
