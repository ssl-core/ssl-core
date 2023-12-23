import React, { useState } from "react";

const App = () => {
  const countState = useState(1);
  const count = countState[0];
  const setCount = countState[1];

  const increment = () => {
    alert((count) + " million dollars!");
    setCount((count) => count + 1);
  }
  
  return (<button onClick={increment}>
    Hold the world ransom for...
  </button>);
};

export default App;
