package util

import "reflect"

func SetDefaultIfNil[T any](value T, defaultValue T) T {
	if isNil(value) {
		return defaultValue
	}

	return value
}

func isNil[T any](t T) bool {
	v := reflect.ValueOf(t)
	kind := v.Kind()

	return (kind == reflect.Ptr ||
		kind == reflect.Interface ||
		kind == reflect.Slice ||
		kind == reflect.Map ||
		kind == reflect.Chan ||
		kind == reflect.Func) &&
		v.IsNil()
}
