package main


import (
	// "os"
    // "github.com/InfluxCommunity/influxdb3-go/influxdb3"
	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
)

type MetricsFields struct {
	id string
	vel string
	name string
}

func main() {
	
	token_local := "hot5dyT0c7hRIGYDsjnWXi4tHgeUZNhr2MHD7JuS9RFAGsugvi78g3cxZa1f3nlfODlho57KpXhlVsdw94V-ow=="
	url := "http://localhost:8086"

	token := token_local
	
    client := influxdb2.NewClient(url, token)

	database := "metrics"
	org := "robocin"
	writeAPI := client.WriteAPI(org, database)

	data := map[string]map[string]interface{}{
		"metric1": {
		  "id": "teste",
		  "vel": "1000",
		  "name": "robo1",
		},
		"metric2": {
		  "id": "2",
		  "vel": "20",
		  "name": "robo2",
		},
		"metric3": {
		  "id":"3",
		  "vel":"20",
		  "name": "robo3",
		},
		"metric4": {
		  "id": "4",
		  "vel": "40",
		  "name": "robo4",
		},
		"metric5": {
		  "id": "5",
		  "vel": "50",
		  "name": "robo5",
		},
	}

	key:="metric2"
	point := influxdb2.NewPointWithMeasurement("metrics-rc").AddTag("id", data[key]["id"].(string)).AddField(data[key]["name"].(string), data[key]["vel"].(string))

	writeAPI.WritePoint(point)

    // Ensure all pending writes are flushed
    writeAPI.Flush()

}