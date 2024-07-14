import { BallResponse } from "../types/requests";

class Ball {
  private confidence: number;
  private position: Vector3;
  private velocity: Vector3;

  constructor(data: BallResponse) {
    this.confidence = data.confidence;
    this.position = data.position;
    this.velocity = data.velocity;
  }
}

export default Ball;
