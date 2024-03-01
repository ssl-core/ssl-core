import {
  LineMaterial,
  LineSegments2,
  LineSegmentsGeometry,
} from "three/examples/jsm/Addons.js";

import constants from "../../../../../../config/constants";

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
      .translate(0, 0, 0.001);
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
    const outsideMargin = constants.field.outside.margin;
    const penaltyWidth = constants.field.penalty.width;
    const penaltyHeight = constants.field.penalty.height;

    return [
      -fieldWidthHalf - outsideMargin,
      -penaltyHeight / 2.0,
      0,
      -fieldWidthHalf + penaltyWidth,
      -penaltyHeight / 2.0,
      0,

      -fieldWidthHalf + penaltyWidth,
      -penaltyHeight / 2.0,
      0,
      -fieldWidthHalf + penaltyWidth,
      penaltyHeight / 2.0,
      0,

      -fieldWidthHalf + penaltyWidth,
      penaltyHeight / 2.0,
      0,
      -fieldWidthHalf - outsideMargin,
      penaltyHeight / 2.0,
      0,
    ];
  }

  private rightPenaltyPoints() {
    const fieldWidthHalf = constants.field.width / 2.0;
    const outsideMargin = constants.field.outside.margin;
    const penaltyWidth = constants.field.penalty.width;
    const penaltyHeight = constants.field.penalty.height;

    return [
      fieldWidthHalf + outsideMargin,
      -penaltyHeight / 2.0,
      0,
      fieldWidthHalf - penaltyWidth,
      -penaltyHeight / 2.0,
      0,

      fieldWidthHalf - penaltyWidth,
      -penaltyHeight / 2.0,
      0,
      fieldWidthHalf - penaltyWidth,
      penaltyHeight / 2.0,
      0,

      fieldWidthHalf - penaltyWidth,
      penaltyHeight / 2.0,
      0,
      fieldWidthHalf + outsideMargin,
      penaltyHeight / 2.0,
      0,
    ];
  }

  private midLinesPoints() {
    const fieldWidthHalf = constants.field.width / 2.0;
    const fieldHeightHalf = constants.field.height / 2.0;
    const outsideMargin = constants.field.outside.margin;

    return [
      -fieldWidthHalf - outsideMargin,
      0,
      0,
      fieldWidthHalf + outsideMargin,
      0,
      0,

      0,
      -fieldHeightHalf - outsideMargin,
      0,
      0,
      fieldHeightHalf + outsideMargin,
      0,
    ];
  }

  private centerCirclePoints() {
    const centerRadius = constants.field.center.radius;
    const segments = constants.field.segments;
    const points = [];

    for (let i = 0; i < segments; i++) {
      const angle = (i / segments) * Math.PI * 2;
      points.push(
        Math.cos(angle) * centerRadius,
        Math.sin(angle) * centerRadius,
        0
      );

      if (i > 0) {
        points.push(
          Math.cos(angle) * centerRadius,
          Math.sin(angle) * centerRadius,
          0
        );
      }
    }

    points.push(Math.cos(0) * centerRadius, Math.sin(0) * centerRadius, 0);

    return points;
  }
}

export default ThreeFieldLinesMesh;
