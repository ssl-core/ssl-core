package concurrency

import (
	"sync"
	"time"
)

type ConcurrentQueue[T any] struct {
	items []T
	mutex sync.Mutex
	cond  *sync.Cond
}

func NewQueue[T any]() *ConcurrentQueue[T] {
	q := &ConcurrentQueue[T]{}
	q.cond = sync.NewCond(&q.mutex)
	return q
}

func (q *ConcurrentQueue[T]) Enqueue(item T) {
	q.mutex.Lock()
	defer q.mutex.Unlock()
	q.items = append(q.items, item)

	q.cond.Signal()
}

func (q *ConcurrentQueue[T]) DequeueAll() []T {
	q.mutex.Lock()
	defer q.mutex.Unlock()

	for len(q.items) == 0 {
		q.cond.Wait()
	}

	items := q.items
	q.items = nil
	return items
}

func (q *ConcurrentQueue[T]) DequeueAllWait(millis int64) []T {
	time.Sleep(time.Millisecond * time.Duration(millis))

	q.mutex.Lock()
	defer q.mutex.Unlock()

	items := q.items
	q.items = nil
	return items
}

func (q *ConcurrentQueue[T]) DequeueAllWaitLen(n int) []T {
	q.mutex.Lock()
	defer q.mutex.Unlock()

	for len(q.items) < n {
		q.cond.Wait()
	}

	items := q.items
	q.items = nil
	return items
}

func (q *ConcurrentQueue[T]) Len() int {
	q.mutex.Lock()
	defer q.mutex.Unlock()
	return len(q.items)
}
