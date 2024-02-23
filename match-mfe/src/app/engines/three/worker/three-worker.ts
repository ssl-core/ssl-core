import ThreeSceneManager from "./three-scene-manager";
import ThreeEventHandler from "./three-event-handler";

const sceneManager = new ThreeSceneManager();
const handler = new ThreeEventHandler(sceneManager);

self.onmessage = (event) => {
  const { data } = event;
  handler.handleEvent(data.type, data.payload);
};
