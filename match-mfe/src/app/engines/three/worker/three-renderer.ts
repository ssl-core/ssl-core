import {
  BoxGeometry,
  DirectionalLight,
  Mesh,
  MeshPhongMaterial,
  PerspectiveCamera,
  Scene,
  WebGLRenderer,
} from "three";

class ThreeRenderer {
  private canvas: OffscreenCanvas | null;
  private renderer: WebGLRenderer | null;
  private camera: PerspectiveCamera;
  private scene: Scene;

  constructor() {
    this.canvas = null;
    this.renderer = null;
    this.camera = new PerspectiveCamera(75, 2, 0.1, 100);
    this.scene = new Scene();
  }

  public initialize(canvas: OffscreenCanvas) {
    this.canvas = canvas;
    this.renderer = new WebGLRenderer({ canvas });
    this.camera.position.z = 4;

    this.setLight();
    this.createBox(0x44aa88, 0);
    this.createBox(0x8844aa, -2);
    this.createBox(0xaa8844, 2);
    this.renderer.render(this.scene, this.camera);
  }

  public resize(width: number, height: number) {
    if (!this.canvas || !this.renderer) {
      throw new Error("Renderer not initialized");
    }

    this.canvas.width = width;
    this.canvas.height = height;
    this.camera.aspect = width / height;
    this.camera.updateProjectionMatrix();
    this.renderer.render(this.scene, this.camera);
  }

  public renderFrame(_match: Match): void {
    if (!this.renderer) {
      throw new Error("Renderer not initialized");
    }

    this.scene.children.forEach((child, index) => {
      if (child instanceof Mesh) {
        const speed = 1 + index * 0.1;
        const rot = speed * Date.now();
        child.rotation.x = rot;
        child.rotation.y = rot;
      }
    });

    this.renderer.render(this.scene, this.camera);
  }

  private setLight() {
    const color = 0xffffff;
    const intensity = 1;
    const light = new DirectionalLight(color, intensity);
    light.position.set(-1, 2, 4);
    this.scene.add(light);
  }

  private createBox(color: number, x: number) {
    const geometry = new BoxGeometry(1, 1, 1);
    const material = new MeshPhongMaterial({ color });
    const cube = new Mesh(geometry, material);
    this.scene.add(cube);
    cube.position.x = x;
  }
}

export default ThreeRenderer;
