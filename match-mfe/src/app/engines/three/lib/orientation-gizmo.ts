import { Euler, Matrix4, Vector3 } from "three";

class OrientationGizmo {
  private static readonly options = {
    size: 90,
    padding: 8,
    bubbleSizePrimary: 8,
    bubbleSizeSecondary: 6,
    showSecondary: true,
    lineWidth: 2,
    fontSize: "11px",
    fontFamily: "arial",
    fontWeight: "bold",
    fontColor: "#151515",
    fontYAdjust: 0,
    colors: {
      x: ["#f73c3c", "#942424"],
      y: ["#6ccb26", "#417a17"],
      z: ["#178cf0", "#0e5490"],
    },
  };

  private bubbles: any[];
  private center: Vector3;
  private onAxisSelected: ((axis: any) => void) | null;
  private selectedAxis: any;
  private canvas: HTMLCanvasElement;
  private context: CanvasRenderingContext2D;
  private mouse: Vector3 | null;

  constructor(parent: ShadowRoot | HTMLElement) {
    this.bubbles = [
      {
        axis: "x",
        direction: new Vector3(1, 0, 0),
        size: OrientationGizmo.options.bubbleSizePrimary,
        color: OrientationGizmo.options.colors.x,
        line: OrientationGizmo.options.lineWidth,
        label: "X",
      },
      {
        axis: "y",
        direction: new Vector3(0, 1, 0),
        size: OrientationGizmo.options.bubbleSizePrimary,
        color: OrientationGizmo.options.colors.y,
        line: OrientationGizmo.options.lineWidth,
        label: "Y",
      },
      {
        axis: "z",
        direction: new Vector3(0, 0, 1),
        size: OrientationGizmo.options.bubbleSizePrimary,
        color: OrientationGizmo.options.colors.z,
        line: OrientationGizmo.options.lineWidth,
        label: "Z",
      },
      {
        axis: "-x",
        direction: new Vector3(-1, 0, 0),
        size: OrientationGizmo.options.bubbleSizeSecondary,
        color: OrientationGizmo.options.colors.x,
      },
      {
        axis: "-y",
        direction: new Vector3(0, -1, 0),
        size: OrientationGizmo.options.bubbleSizeSecondary,
        color: OrientationGizmo.options.colors.y,
      },
      {
        axis: "-z",
        direction: new Vector3(0, 0, -1),
        size: OrientationGizmo.options.bubbleSizeSecondary,
        color: OrientationGizmo.options.colors.z,
      },
    ];
    this.center = new Vector3(
      OrientationGizmo.options.size / 2,
      OrientationGizmo.options.size / 2,
      0
    );
    this.mouse = null;
    this.onAxisSelected = null;
    this.selectedAxis = null;
    this.canvas = document.createElement("canvas");
    this.canvas.style.position = "absolute";
    this.canvas.style.top = "0.5rem";
    this.canvas.style.right = "0.5rem";
    this.context = this.canvas.getContext("2d")!;
    this.canvas.width = OrientationGizmo.options.size;
    this.canvas.height = OrientationGizmo.options.size;
    parent.appendChild(this.canvas);
  }

  public initialize() {
    this.canvas.addEventListener(
      "mousemove",
      (e) => this.onMouseMove(e),
      false
    );
    this.canvas.addEventListener("mouseout", () => this.onMouseOut(), false);
    this.canvas.addEventListener("click", () => this.onMouseClick(), false);
  }

  public terminate() {
    this.canvas.removeEventListener(
      "mousemove",
      (e) => this.onMouseMove(e),
      false
    );
    this.canvas.removeEventListener("mouseout", () => this.onMouseOut(), false);
    this.canvas.removeEventListener("click", () => this.onMouseClick(), false);
  }

  public update(rotation: any) {
    this.clear();

    let rotMat = new Matrix4().makeRotationFromEuler(
      new Euler().fromArray(rotation)
    );
    let invRotMat = rotMat.clone().invert();

    for (let bubble of this.bubbles) {
      bubble.position = this.getBubblePosition(
        bubble.direction.clone().applyMatrix4(invRotMat)
      );
    }

    const layers = [];
    for (let axis in this.bubbles) {
      if (OrientationGizmo.options.showSecondary === true || axis[0] !== "-") {
        layers.push(this.bubbles[axis]);
      }
    }

    layers.sort((a, b) => (a.position.z > b.position.z ? 1 : -1));

    this.selectedAxis = null;

    if (this.mouse) {
      let closestDist = Infinity;

      for (let bubble of layers) {
        const distance = this.mouse.distanceTo(bubble.position);

        if (distance < closestDist || distance < bubble.size) {
          closestDist = distance;
          this.selectedAxis = bubble;
        }
      }
    }

    this.drawLayers(layers);
  }

  public setOnAxisSelected(callback: (axis: any) => void) {
    this.onAxisSelected = callback;
  }

  private onMouseMove(e: MouseEvent) {
    const rect = this.canvas.getBoundingClientRect();
    this.mouse = new Vector3(e.clientX - rect.left, e.clientY - rect.top, 0);
  }

  private onMouseOut() {
    this.mouse = null;
  }

  private onMouseClick() {
    if (!!this.onAxisSelected && typeof this.onAxisSelected == "function") {
      this.onAxisSelected({
        axis: this.selectedAxis.axis,
        direction: this.selectedAxis.direction.clone(),
      });
    }
  }

  private clear() {
    this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);
  }

  private drawCircle(p: Vector3, radius = 10, color = "#FF0000") {
    this.context.beginPath();
    this.context.arc(p.x, p.y, radius, 0, 2 * Math.PI, false);
    this.context.fillStyle = color;
    this.context.fill();
    this.context.closePath();
  }

  private drawLine(p1: Vector3, p2: Vector3, width = 1, color = "#FF0000") {
    this.context.beginPath();
    this.context.moveTo(p1.x, p1.y);
    this.context.lineTo(p2.x, p2.y);
    this.context.lineWidth = width;
    this.context.strokeStyle = color;
    this.context.stroke();
    this.context.closePath();
  }

  private drawLayers(layers: any[]) {
    for (let bubble of layers) {
      let color = bubble.color;

      if (this.selectedAxis === bubble) {
        color = "#FFFFFF";
      } else if (bubble.position.z >= -0.01) {
        color = bubble.color[0];
      } else {
        color = bubble.color[1];
      }

      this.drawCircle(bubble.position, bubble.size, color);

      if (bubble.line) {
        this.drawLine(this.center, bubble.position, bubble.line, color);
      }

      if (bubble.label) {
        this.context.font = [
          OrientationGizmo.options.fontWeight,
          OrientationGizmo.options.fontSize,
          OrientationGizmo.options.fontFamily,
        ].join(" ");
        this.context.fillStyle = OrientationGizmo.options.fontColor;
        this.context.textBaseline = "middle";
        this.context.textAlign = "center";
        this.context.fillText(
          bubble.label,
          bubble.position.x,
          bubble.position.y + OrientationGizmo.options.fontYAdjust
        );
      }
    }
  }

  private getBubblePosition(position: Vector3) {
    return new Vector3(
      position.x *
        (this.center.x -
          OrientationGizmo.options.bubbleSizePrimary / 2 -
          OrientationGizmo.options.padding) +
        this.center.x,
      this.center.y -
        position.y *
          (this.center.y -
            OrientationGizmo.options.bubbleSizePrimary / 2 -
            OrientationGizmo.options.padding),
      position.z
    );
  }
}

export default OrientationGizmo;
