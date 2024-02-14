const sleep = (ms: number) => {
  var now = new Date().getTime();
  while (new Date().getTime() < now + ms);
};

while (true) {
  self.postMessage({ type: "tick" });
  sleep(10);
}
