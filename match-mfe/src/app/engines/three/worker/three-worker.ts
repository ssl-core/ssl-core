import ThreeRenderer from "./three-renderer";
import ThreeEventHandler from "./three-event-handler";

const renderer = new ThreeRenderer();
const handler = new ThreeEventHandler(renderer);

self.onmessage = (event) => {
  const { data } = event;
  handler.handleEvent(data.type, data.payload);
};
