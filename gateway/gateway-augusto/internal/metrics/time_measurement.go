package metrics

import (
	"fmt"
	"sync"
	"time"

	"github.com/robocin/ssl-core/gateway/gateway-augusto/internal/utils"
)

type TimeMeasurementState int

const (
	waitingFirstMessagesToStart TimeMeasurementState = iota
	waitingStartTime
	waitingEndTime
	endMeasurement
)

type TimeMeasure struct {
	start time.Time
	end   time.Time
}

func NewTimeMeasure(start time.Time, end time.Time) TimeMeasure {
	return TimeMeasure{
		start: start,
		end:   end,
	}
}

type TimeMeasurement struct {
	state        TimeMeasurementState
	startTime    time.Time
	endTime      time.Time
	mu           sync.Mutex
	messageCount int
	waitCount    int
	timestamps   []TimeMeasure
	measureCount int
}

func NewTimeMeasurement(waitCount int, measureCount int) *TimeMeasurement {
	return &TimeMeasurement{
		state:        waitingFirstMessagesToStart,
		startTime:    time.Time{},
		endTime:      time.Time{},
		mu:           sync.Mutex{},
		messageCount: 0,
		waitCount:    waitCount,
		timestamps:   []TimeMeasure{},
		measureCount: measureCount,
	}
}

func (tm *TimeMeasurement) SaveStartTime() bool {
	tm.mu.Lock()
	defer tm.mu.Unlock()

	timestamp := time.Now()
	switch tm.state {
	case waitingFirstMessagesToStart:
		tm.messageCount++
		if tm.messageCount >= tm.waitCount {
			tm.state = waitingStartTime
		}
		return false
	case waitingStartTime:
		tm.startTime = timestamp
		tm.state = waitingEndTime
	}

	return true
}

func (tm *TimeMeasurement) SaveEndTime() bool {
	tm.mu.Lock()
	defer tm.mu.Unlock()

	timestamp := time.Now()
	switch tm.state {
	case waitingEndTime:
		tm.endTime = timestamp
		tm.timestamps = append(tm.timestamps, NewTimeMeasure(tm.startTime, tm.endTime))

		if len(tm.timestamps) >= tm.measureCount {
			tm.state = endMeasurement
		} else {
			tm.state = waitingStartTime
		}
	case endMeasurement:
		fmt.Println("Measurement already completed. Exporting to CSV.")
		tm.exportToCSV(fmt.Sprint("time_measurement_%d.csv", timestamp.Unix()))
		return false
	}

	return true
}

func (tm *TimeMeasurement) exportToCSV(filename string) error {
	header := []string{"Start Time", "End Time", "Duration (ms)"}
	duration := tm.endTime.Sub(tm.startTime).Milliseconds()
	data := [][]string{
		{tm.startTime.String(), tm.endTime.String(), fmt.Sprintf("%d", duration)},
	}

	return utils.CSVExporter(filename, header, data)
}
