import { RobotResponse } from "../types/requests";

class Robot {
  private confidence: number;
  private robotId: number;
  private robotColor: RobotColor;
  private position: Vector2;
  private angle: number;
  private velocity: Vector2;
  private angularVelocity: number;
  private radius: number;
  private height: number;
  private dribblerWidth: number;

  constructor(data: RobotResponse) {
    this.confidence = data.confidence;
    this.robotId = data.robot_id;
    this.robotColor = data.robot_color;
    this.position = data.position;
    this.angle = data.angle;
    this.velocity = data.velocity;
    this.angularVelocity = data.angular_velocity;
    this.radius = data.radius;
    this.height = data.height;
    this.dribblerWidth = data.dribbler_width;
  }
}

export default Robot;
