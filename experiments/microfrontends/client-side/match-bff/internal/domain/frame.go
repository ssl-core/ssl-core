package domain

type Frame struct {
	id        string
	timestamp int64
	robots    []Robot
	ball      Ball
}
