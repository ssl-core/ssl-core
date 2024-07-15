package time_util

import (
	"time"

	"google.golang.org/protobuf/types/known/timestamppb"
)

// Replacement for .AsTime() due noticed precision errors.
func TimeFromTimestamp(ts *timestamppb.Timestamp) time.Time {
	return time.Unix(ts.GetSeconds(), int64(ts.GetNanos()))
}
