package service

import (
	"fmt"
)

type MatchService struct {
}

func (ms MatchService) Find() {
	fmt.Println("got path")
}
