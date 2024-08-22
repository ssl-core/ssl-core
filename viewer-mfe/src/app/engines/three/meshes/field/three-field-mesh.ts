import { MeshLambertMaterial, PlaneGeometry } from "three";

import ThreeBaseMesh from "../three-base-mesh";
import constants from "../../../../../config/constants";

class ThreeFieldMesh extends ThreeBaseMesh {
  private length: number;
  private width: number;
  private boundary: number;

  constructor(length?: number, width?: number, boundary?: number) {
    super();
    this.receiveShadow = true;
    this.length = length || 0;
    this.width = width || 0;
    this.boundary = boundary || 0;

    this.buildMesh();
  }

  public setParams(length: number, width: number, boundary: number) {
    this.length = length;
    this.width = width;
    this.boundary = boundary;

    this.buildMesh();
  }

  protected buildGeometry() {
    const length = this.length + this.boundary * 2;
    const width = this.width + this.boundary * 2;

    return new PlaneGeometry(length, width);
  }

  protected buildMaterial() {
    return new MeshLambertMaterial({
      color: constants.field.color,
      fog: false,
    });
  }
}

export default ThreeFieldMesh;
