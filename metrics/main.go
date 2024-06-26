package main


import (
	"os"
    // "github.com/InfluxCommunity/influxdb3-go/influxdb3"
	influxdb2 "github.com/influxdata/influxdb-client-go/v2"
)

type MetricsFields struct {
	id string
	vel string
	name string
}

func main() {
	// input:= MetricsFields{
	// 	id: "1",
	// 	vel: "10",
	// 	name: "robo1",
	// }
	url := "https://us-east-1-1.aws.cloud2.influxdata.com"
    token := os.Getenv("INFLUXDB_TOKEN")
	
    // Create a new client using an InfluxDB server base URL and an authentication token
    client := influxdb2.NewClient(url, token)


	database := "bucket-rc"

	writeAPI := client.WriteAPI("ufpe", database)

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

	key:="metric1"
	point := influxdb2.NewPointWithMeasurement("metrics").AddTag("id", data[key]["id"].(string)).AddField(data[key]["vel"].(string), data[key]["name"].(string))

	writeAPI.WritePoint(point)

    // Ensure all pending writes are flushed
    writeAPI.Flush()

}