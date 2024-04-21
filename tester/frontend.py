from selenium import webdriver
from selenium.webdriver.chrome.service import Service as ChromeService
from webdriver_manager.chrome import ChromeDriverManager
from datetime import datetime
import csv

# Download the driver
service = ChromeService(ChromeDriverManager().install(), service_args=["--disable-gpu-vsync"])

# Start the driver and open the browser
driver = webdriver.Chrome(service=service)

# Open the URL
driver.get("http://localhost:3031/")

# Set timeout to 30 minutes
driver.set_script_timeout(30 * 60)

# Execute the JavaScript code
script = """
let callback = arguments[arguments.length - 1];
document.body.addEventListener("tester", (event) => {
  callback(event.detail);
});
"""

result = driver.execute_async_script(script)

# Save the result to a CSV file
today = datetime.now().strftime("%Y%m%d%H%M%S")
path = f"results/{today}.csv"
with open(path, "w", newline="", encoding="utf-8") as file:
  writer = csv.writer(file)
  writer.writerow(["duration", "start", "end"])
  writer.writerows([[row['duration'], row['startTime'], row['endTime']] for row in result])

# Print the result
print(f"Result saved to '{path}'")

# Close the browser
driver.quit()
