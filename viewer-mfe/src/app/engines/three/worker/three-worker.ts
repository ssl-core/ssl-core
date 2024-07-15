import ThreeSceneManager from "../scene/three-scene-manager";
import ThreeEventHandler from "./three-event-handler";
import ThreeProxyManager from "../proxy/three-proxy-manager";

const sceneManager = new ThreeSceneManager();
const proxyManager = new ThreeProxyManager();
const handler = new ThreeEventHandler(sceneManager, proxyManager);

// @ts-ignore
self.document = {
  addEventListener: () => {},
};

self.onmessage = (event) => {
  const { data } = event;
  handler.handleEvent(data.type, data.payload);
};
