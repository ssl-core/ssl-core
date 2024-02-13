import * as three from "three";

import BaseRenderEngine from "../../lib/base-render-engine";

class ThreeRenderEngine extends BaseRenderEngine {
  private scene: three.Scene;
  private camera: three.PerspectiveCamera;
  private renderer: three.WebGLRenderer;

  constructor(root: ShadowRoot) {
    super(root);

    this.scene = new three.Scene();
    this.camera = new three.PerspectiveCamera(75, 1, 0.1, 1000);
    this.renderer = new three.WebGLRenderer();
  }

  public initialize(): void {
    this.camera.position.set(0, 0, 100);
    this.camera.lookAt(0, 0, 0);
    this.renderer.setSize(300, 300);
    this.root.appendChild(this.renderer.domElement);

    // Test rendering a line
    const material = new three.LineBasicMaterial({ color: "#ff00ff" });
    const points = [];
    points.push(new three.Vector3(-10, 0, 0));
    points.push(new three.Vector3(0, 10, 0));
    points.push(new three.Vector3(10, 0, 0));

    const geometry = new three.BufferGeometry().setFromPoints(points);
    const line = new three.Line(geometry, material);
    this.scene.add(line);
    this.renderer.render(this.scene, this.camera);
  }

  public render(match: Match): void {
    console.log(match);
  }
}

export default ThreeRenderEngine;
