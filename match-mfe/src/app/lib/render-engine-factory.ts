import BaseRenderEngine from "./base-render-engine";
import ThreeRenderEngine from "../engines/three/three-render-engine";

class RenderEngineFactory {
  static createEngine(type: string, root: ShadowRoot): BaseRenderEngine {
    switch (type) {
      case "three":
        return new ThreeRenderEngine(root);
    }

    throw new Error(`Unknown render engine type: ${type}`);
  }
}

export default RenderEngineFactory;
