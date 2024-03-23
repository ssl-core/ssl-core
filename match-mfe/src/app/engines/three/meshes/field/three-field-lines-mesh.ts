import {
  LineMaterial,
  LineSegments2,
  LineSegmentsGeometry,
} from "three/examples/jsm/Addons.js";

import constants from "../../../../../config/constants";

class ThreeFieldLinesMesh extends LineSegments2 {
  constructor() {
    super();
    this.geometry = this.buildGeometry();
    this.material = this.buildMaterial();
  }

  private buildGeometry() {
    const points = [
      ...this.fieldPoints(),
      ...this.penaltyPoints(1),
      ...this.penaltyPoints(-1),
      ...this.midLinesPoints(),
      ...this.centerCirclePoints(),
    ];

    return new LineSegmentsGeometry()
      .setPositions(points)
      .translate(0, 0, constants.clippingEpsilon);
  }

  private buildMaterial() {
    return new LineMaterial({
      color: constants.field.line.color,
      linewidth: constants.field.line.width,
      worldUnits: true,
    });
  }

  private fieldPoints() {
    const fieldHalfWidth = constants.field.width / 2;
    const fieldHalfHeight = constants.field.height / 2;

    return [
      -fieldHalfWidth,
      -fieldHalfHeight,
      0,
      -fieldHalfWidth,
      fieldHalfHeight,
      0,

      -fieldHalfWidth,
      fieldHalfHeight,
      0,
      fieldHalfWidth,
      fieldHalfHeight,
      0,

      fieldHalfWidth,
      fieldHalfHeight,
      0,
      fieldHalfWidth,
      -fieldHalfHeight,
      0,

      fieldHalfWidth,
      -fieldHalfHeight,
      0,
      -fieldHalfWidth,
      -fieldHalfHeight,
      0,
    ];
  }

  private penaltyPoints(side: number) {
    const fieldHalfWidth = (side * constants.field.width) / 2;
    const penaltyWidth = -side * constants.field.penalty.width;
    const penaltyHeight = constants.field.penalty.height;
    const penaltyHalfHeight = penaltyHeight / 2;

    return [
      fieldHalfWidth,
      -penaltyHalfHeight,
      0,
      fieldHalfWidth + penaltyWidth,
      -penaltyHalfHeight,
      0,

      fieldHalfWidth + penaltyWidth,
      -penaltyHalfHeight,
      0,
      fieldHalfWidth + penaltyWidth,
      penaltyHalfHeight,
      0,

      fieldHalfWidth + penaltyWidth,
      penaltyHalfHeight,
      0,
      fieldHalfWidth,
      penaltyHalfHeight,
      0,
    ];
  }

  private midLinesPoints() {
    const fieldHalfWidth = constants.field.width / 2;
    const fieldHalfHeight = constants.field.height / 2;

    return [
      -fieldHalfWidth,
      0,
      0,
      fieldHalfWidth,
      0,
      0,

      0,
      -fieldHalfHeight,
      0,
      0,
      fieldHalfHeight,
      0,
    ];
  }

  private centerCirclePoints() {
    const { radius, segments } = constants.field.center;
    const points = [];

    for (let segmentIndex = 0; segmentIndex < segments; segmentIndex++) {
      const angle = (segmentIndex / segments) * Math.PI * 2;
      points.push(Math.cos(angle) * radius, Math.sin(angle) * radius, 0);

      if (segmentIndex > 0) {
        points.push(Math.cos(angle) * radius, Math.sin(angle) * radius, 0);
      }
    }

    points.push(Math.cos(0) * radius, Math.sin(0) * radius, 0);

    return points;
  }
}

export default ThreeFieldLinesMesh;
