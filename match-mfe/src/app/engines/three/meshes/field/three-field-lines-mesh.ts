import {
  LineMaterial,
  LineSegments2,
  LineSegmentsGeometry,
} from "three/addons";

import constants from "../../../../../config/constants";

class ThreeFieldLinesMesh extends LineSegments2 {
  private length: number;
  private width: number;
  private penaltyDepth: number;
  private penaltyWidth: number;

  constructor(
    length?: number,
    width?: number,
    penaltyDepth?: number,
    penaltyWidth?: number
  ) {
    super();
    this.receiveShadow = true;
    this.length = length || 0;
    this.width = width || 0;
    this.penaltyDepth = penaltyDepth || 0;
    this.penaltyWidth = penaltyWidth || 0;

    this.buildMesh();
  }

  public setParams(
    length: number,
    width: number,
    penaltyDepth: number,
    penaltyWidth: number
  ) {
    this.length = length;
    this.width = width;
    this.penaltyDepth = penaltyDepth;
    this.penaltyWidth = penaltyWidth;

    this.buildMesh();
  }

  private buildMesh() {
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

    return new LineSegmentsGeometry().setPositions(points);
  }

  private buildMaterial() {
    return new LineMaterial({
      color: constants.field.line.color,
      linewidth: constants.field.line.width,
      worldUnits: true,
      dashed: false,
      alphaToCoverage: true,
      toneMapped: false,
      polygonOffset: true,
      polygonOffsetFactor: 1,
      polygonOffsetUnits: 1,
    });
  }

  private fieldPoints() {
    const fieldHalfLength = this.length / 2;
    const fieldHalfWidth = this.width / 2;

    return [
      -fieldHalfLength,
      -fieldHalfWidth,
      0,
      -fieldHalfLength,
      fieldHalfWidth,
      0,

      -fieldHalfLength,
      fieldHalfWidth,
      0,
      fieldHalfLength,
      fieldHalfWidth,
      0,

      fieldHalfLength,
      fieldHalfWidth,
      0,
      fieldHalfLength,
      -fieldHalfWidth,
      0,

      fieldHalfLength,
      -fieldHalfWidth,
      0,
      -fieldHalfLength,
      -fieldHalfWidth,
      0,
    ];
  }

  private penaltyPoints(side: number) {
    const fieldHalfLength = (side * this.length) / 2;
    const penaltyDepth = -side * this.penaltyDepth;
    const penaltyWidth = this.penaltyWidth;
    const penaltyHalfWidth = penaltyWidth / 2;

    return [
      fieldHalfLength,
      -penaltyHalfWidth,
      0,
      fieldHalfLength + penaltyDepth,
      -penaltyHalfWidth,
      0,

      fieldHalfLength + penaltyDepth,
      -penaltyHalfWidth,
      0,
      fieldHalfLength + penaltyDepth,
      penaltyHalfWidth,
      0,

      fieldHalfLength + penaltyDepth,
      penaltyHalfWidth,
      0,
      fieldHalfLength,
      penaltyHalfWidth,
      0,
    ];
  }

  private midLinesPoints() {
    const fieldHalfLength = this.length / 2;
    const fieldHalfWidth = this.width / 2;

    return [
      -fieldHalfLength,
      0,
      0,
      fieldHalfLength,
      0,
      0,

      0,
      -fieldHalfWidth,
      0,
      0,
      fieldHalfWidth,
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
