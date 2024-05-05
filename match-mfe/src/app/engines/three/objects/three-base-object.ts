import { Group } from "three";

abstract class ThreeBaseObject extends Group {
  protected selectable: boolean;
  protected selected: boolean;

  constructor() {
    super();
    this.selected = false;
    this.selectable = false;
  }

  public isSelectable() {
    return this.selectable;
  }

  public isSelected() {
    return this.selectable && this.selected;
  }

  public select() {
    if (!this.selectable) {
      throw new Error("Object is not selectable");
    }

    this.selected = true;
    this.onSelect();
  }

  public deselect() {
    if (!this.selectable) {
      throw new Error("Object is not selectable");
    }

    this.selected = false;
    this.onDeselect();
  }

  protected onSelect() {}

  protected onDeselect() {}

  public abstract update(): void;
  protected abstract addMeshes(): void;
}

export default ThreeBaseObject;
