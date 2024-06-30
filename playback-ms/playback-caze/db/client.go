package db

type DBClient interface {
	Get(string) (interface{}, error)
	GetRange(string, string) (map[string]interface{}, error)
	Set(string, interface{}) error
	SetMany(map[string]interface{}) error
}
