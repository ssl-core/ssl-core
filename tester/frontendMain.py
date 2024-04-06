from selenium import webdriver
from selenium.webdriver.chrome.service import Service as ChromeService
from webdriver_manager.chrome import ChromeDriverManager

# Download the driver
service = ChromeService(ChromeDriverManager().install())

# Start the driver and open the browser
driver = webdriver.Chrome(service=service)

# Open the URL
driver.get("https://echo.websocket.org/")

# Execute the JavaScript code
script = """
let callback = arguments[arguments.length - 1];

let socket = new WebSocket("wss://echo.websocket.org/");

let start;
socket.onopen = function(e) {
  start = performance.now();
  socket.send("a");
};

socket.onmessage = function(e) {
  if (e.data === "a") {
    callback(performance.now() - start);
  }
};
"""

result = driver.execute_async_script(script)

# Print the result
print(result)

# Close the browser
driver.quit()
