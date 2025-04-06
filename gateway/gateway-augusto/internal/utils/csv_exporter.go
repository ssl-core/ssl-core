package utils

import (
	"encoding/csv"
	"fmt"
	"os"
)

func CSVExporter(filename string, header []string, data [][]string) error {
	file, err := os.Create(filename)
	if err != nil {
		return fmt.Errorf("failed to create CSV file: %v", err)
	}
	defer file.Close()

	writer := csv.NewWriter(file)
	defer writer.Flush()

	// Write header
	if err := writer.Write(header); err != nil {
		return fmt.Errorf("failed to write CSV header: %v", err)
	}

	// Write data rows
	for _, row := range data {
		if err := writer.Write(row); err != nil {
			return fmt.Errorf("failed to write CSV data: %v", err)
		}
	}

	fmt.Println("Data exported to", filename)
	return nil
}
