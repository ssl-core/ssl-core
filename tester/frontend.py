from selenium import webdriver
from selenium.webdriver.chrome.service import Service as ChromeService
from webdriver_manager.chrome import ChromeDriverManager
from datetime import datetime
import csv

# Download the driver
service = ChromeService(ChromeDriverManager().install())

# Start the driver and open the browser
driver = webdriver.Chrome(service=service)

# Open the URL
driver.get("http://localhost:3031/")

# Execute the JavaScript code
script = """
const SAMPLES = 100;

let callback = arguments[arguments.length - 1];
const buffer = [];

document.body.addEventListener("tester", (event) => {
  buffer.push(Object.values(event.detail).join());
  
  if (buffer.length === SAMPLES) {
    callback(buffer);
  }
});
"""

result = driver.execute_async_script(script)

# Save the result to a CSV file
today = datetime.now().strftime("%Y%m%d%H%M%S")
path = f"results/{today}.csv"
with open(path, "w", newline="", encoding="utf-8") as file:
  writer = csv.writer(file)
  writer.writerow(["duration", "start", "end"])
  writer.writerows([row.split(",") for row in result])

# Print the result
print(f"Result saved to '{path}'")

# Close the browser
driver.quit()
