package domain

type Color uint8

const (
	Unspecified Color = iota
	Yellow
	Blue
)

type Robot struct {
	id          int
	color       Color
	position    Point2D
	orientation float32
	velocity    Point2D
	angularVel  float32
}
