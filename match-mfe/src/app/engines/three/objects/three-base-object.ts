import { Group } from "three";

abstract class ThreeBaseObject extends Group {
  public abstract update(): void;
  protected abstract addMeshes(): void;
}

export default ThreeBaseObject;
