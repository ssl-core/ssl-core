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
      ...this.leftPenaltyPoints(),
      ...this.rightPenaltyPoints(),
      ...this.midLinesPoints(),
      ...this.centerCirclePoints(),
    ];

    return new LineSegmentsGeometry()
      .setPositions(points)
      .translate(0, 0, 0.01);
  }

  private buildMaterial() {
    return new LineMaterial({
      color: constants.field.line.color,
      linewidth: constants.field.line.width,
      worldUnits: true,
    });
  }

  private fieldPoints() {
    const fieldWidthHalf = constants.field.width / 2.0;
    const fieldHeightHalf = constants.field.height / 2.0;

    return [
      -fieldWidthHalf,
      -fieldHeightHalf,
      0,
      -fieldWidthHalf,
      fieldHeightHalf,
      0,

      -fieldWidthHalf,
      fieldHeightHalf,
      0,
      fieldWidthHalf,
      fieldHeightHalf,
      0,

      fieldWidthHalf,
      fieldHeightHalf,
      0,
      fieldWidthHalf,
      -fieldHeightHalf,
      0,

      fieldWidthHalf,
      -fieldHeightHalf,
      0,
      -fieldWidthHalf,
      -fieldHeightHalf,
      0,
    ];
  }

  private leftPenaltyPoints() {
    const fieldWidthHalf = constants.field.width / 2.0;
    const penaltyWidth = constants.field.penalty.width;
    const penaltyHeight = constants.field.penalty.height;
    const penaltyHeightHalf = penaltyHeight / 2.0;

    return [
      -fieldWidthHalf,
      -penaltyHeightHalf,
      0,
      -fieldWidthHalf + penaltyWidth,
      -penaltyHeightHalf,
      0,

      -fieldWidthHalf + penaltyWidth,
      -penaltyHeightHalf,
      0,
      -fieldWidthHalf + penaltyWidth,
      penaltyHeightHalf,
      0,

      -fieldWidthHalf + penaltyWidth,
      penaltyHeightHalf,
      0,
      -fieldWidthHalf,
      penaltyHeightHalf,
      0,
    ];
  }

  private rightPenaltyPoints() {
    const fieldWidthHalf = constants.field.width / 2.0;
    const penaltyWidth = constants.field.penalty.width;
    const penaltyHeight = constants.field.penalty.height;
    const penaltyHeightHalf = penaltyHeight / 2.0;

    return [
      fieldWidthHalf,
      -penaltyHeightHalf,
      0,
      fieldWidthHalf - penaltyWidth,
      -penaltyHeightHalf,
      0,

      fieldWidthHalf - penaltyWidth,
      -penaltyHeightHalf,
      0,
      fieldWidthHalf - penaltyWidth,
      penaltyHeightHalf,
      0,

      fieldWidthHalf - penaltyWidth,
      penaltyHeightHalf,
      0,
      fieldWidthHalf,
      penaltyHeightHalf,
      0,
    ];
  }

  private midLinesPoints() {
    const fieldWidthHalf = constants.field.width / 2.0;
    const fieldHeightHalf = constants.field.height / 2.0;

    return [
      -fieldWidthHalf,
      0,
      0,
      fieldWidthHalf,
      0,
      0,

      0,
      -fieldHeightHalf,
      0,
      0,
      fieldHeightHalf,
      0,
    ];
  }

  private centerCirclePoints() {
    const { radius, segments } = constants.field.center;
    const points = [];

    for (let i = 0; i < segments; i++) {
      const angle = (i / segments) * Math.PI * 2;
      points.push(Math.cos(angle) * radius, Math.sin(angle) * radius, 0);

      if (i > 0) {
        points.push(Math.cos(angle) * radius, Math.sin(angle) * radius, 0);
      }
    }

    points.push(Math.cos(0) * radius, Math.sin(0) * radius, 0);

    return points;
  }
}

export default ThreeFieldLinesMesh;
